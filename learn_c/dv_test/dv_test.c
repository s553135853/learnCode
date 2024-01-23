#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/netlink.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define NETLINK_TEST 30
#define MSG_LEN 125
#define MAX_PLOAD 125

/*
Audio In:
Step1:MCU请求MCU_MicMeasuringReq=0x1：检测MIC请求;
Step2-1:SOC发送应答指令SOC_MicMeasuringResp=0x00：MIC运行状态OK;
Step2-2:SOC发送应答指令SOC_MicMeasuringResp=0x01：MIC运行状态N_OK;
注:应一直读mic的状态,等待报文来时，回复最近10次的平均值

Audio Out:
Step1:MCU请求MCU_UDiskReq=0x1：请求U盘状态;
Step2:SOC发送应答指令SOC_UDiskSts=0x1：U盘已连接;
Step3:MCU请求MCU_PlayMusicReq=0x1：播放音乐，U盘内第一首后缀以MP3结尾的歌曲;
Step4:SOC发送应答指令SOC_PlayMusicSts=0x1：音乐播放状态;
Step5:MCU请求MCU_PlayMusicReq=0x2：停止播放;
Step6:SOC发送应答指令SOC_PlayMusicSts=0x0：停止播放状态;

生产者-消费者

*/

enum MCU_DATA_COM {
    MUSIC_PLAY = 0x01,
    MUSIC_STOP,
};

#define MUSIC_NAME "test.wav"
#define STORGE_PATH "/storage/"
#define UCARD_PREFIX "udisk-"
#define AK7738_IOCTL_PATH "/dev/ak7738-dsp"
#define AK7738_IOCTL_GETMIR _IOR(0xD0, 0x13, unsigned long)

#define TEMP_MUSIC_PATH "/data/ljj_xlaq.wav"

static pthread_cond_t g_cond;
static pthread_mutex_t g_mutex;

static pthread_cond_t g_cond_play;
static pthread_mutex_t g_mutex_play;

typedef struct _user_msg_info {
    struct nlmsghdr hdr;
    char msg[MSG_LEN];
} user_msg_info;

typedef struct {
    double *array;
    int capacity;
    int front;
    int rear;
} Queue;

// char send_buf[32] = "hello netlink spi...";
char send_buf[11] = {0};
// char send_buf[260] = "hello mcu.";
static char shell_cmd[64];
static char wav_path[64];
static struct nlmsghdr *nlh = NULL;
static int skfd;
static user_msg_info u_info_g;
static pid_t pid;
static int data;
static int pro = 0;

static Queue mic1_queue;
static Queue mic2_queue;
static Queue mic3_queue;
static Queue mic4_queue;

void initQueue(Queue *q, int max_size)
{
    q->array = (double *)malloc(max_size * sizeof(double));
    q->capacity = max_size;
    q->front = 0;
    q->rear = -1;
}

int isFull(Queue *q)
{
    return ((q->rear + 1) % q->capacity) == q->front;
}

int isEmpty(Queue *q)
{
    return q->front == -1;
}

void enqueue(Queue *q, double value)
{
    if (isFull(q)) {
        q->front = (q->front + 1) % q->capacity;
    }

    q->rear = (q->rear + 1) % q->capacity;
    q->array[q->rear] = value;

    if (q->front == -1) {
        q->front = q->rear;
    }
}

int dequeue(Queue *q)
{
    if (isEmpty(q)) {
        printf("queue isEmpty\n");
        return -1;
    }

    int value = q->array[q->front];
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front = (q->front + 1) % q->capacity;
    }
    return value;
}

void traverseQueue(Queue *q, double *result)
{
    if (isEmpty(q)) {
        printf("queue isEmpty\n");
        return;
    }
    int i = q->front;
    do {
        // printf("%f ", q->array[i]);
        *result += q->array[i];
        i = (i + 1) % q->capacity;
    } while (i != (q->rear + 1) % q->capacity);
}

static int receive_data_from_mcu(int fd, user_msg_info *u_info)
{
    socklen_t len;
    struct sockaddr_nl daddr;
    memset(&daddr, 0, sizeof(daddr));
    daddr.nl_family = AF_NETLINK;
    // to kernel
    daddr.nl_pid = 0;
    daddr.nl_groups = 0;
    len = recvfrom(fd, u_info, sizeof(user_msg_info), 0, (struct sockaddr *)&daddr, &len);
    if (len > 0) {
        return len;
    }
    return -1;
}

static int send_data_to_mcu(int fd, struct nlmsghdr *nlh, char *umsg)
{

    int ret;
    struct sockaddr_nl daddr;
    memset(&daddr, 0, sizeof(daddr));
    daddr.nl_family = AF_NETLINK;
    // to kernel
    daddr.nl_pid = 0;
    daddr.nl_groups = 0;
    memcpy(NLMSG_DATA(nlh), umsg, 11);
    ret = sendto(fd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&daddr, sizeof(struct sockaddr_nl));
    if (ret < 0) {
        printf("send msg to mcu failed\n");
    }

    return ret;
}

static void put_queue(long _data, double value)
{
    switch (_data) {
    case 0x6F:
        enqueue(&mic1_queue, value);
        break;
    case 0x70:
        enqueue(&mic2_queue, value);
        break;
    case 0x71:
        enqueue(&mic3_queue, value);
        break;
    case 0x72:
        enqueue(&mic4_queue, value);
        break;
    default:
        break;
    }
}

static int monitor_mic(void *arg)
{
    int fd;
    unsigned long dwMIRData;
    int ret;
    double dB;
    fd = open(AK7738_IOCTL_PATH, O_RDWR);
    if (fd < 0) {
        printf("open /dev/ak7738-dsp ioctl faild\n");
        return -1;
    }
    while (1) {
        for (size_t i = 0; i < 4; i++) {
            dwMIRData = 0x6F + i;
            ret = ioctl(fd, AK7738_IOCTL_GETMIR, &dwMIRData);
            if (ret == 0 && dwMIRData != 0) {
                dB = ((dwMIRData >> 4) / pow(2, 23) - 2) * 16 * 6.02;
                put_queue(0x6F + i, dB);
                usleep(50 * 1000);
            }
        }
    }
}

static int calue_mic_data()
{
    double result;

    traverseQueue(&mic1_queue, &result);
    // printf("result1:%f, /10 :%f\n", result, result / 10);
    if (result / 10 < -15) {
        return 0;
    }
    result = 0;
    traverseQueue(&mic2_queue, &result);
    // printf("result2:%f, /10 :%f\n", result, result / 10);
    if (result / 10 < -15) {
        return 0;
    }
    result = 0;
    traverseQueue(&mic3_queue, &result);
    // printf("result3:%f, /10 :%f\n", result, result / 10);
    if (result / 10 < -15) {
        return 0;
    }
    result = 0;
    traverseQueue(&mic4_queue, &result);
    // printf("result4:%f, /10 :%f\n", result, result / 10);
    if (result / 10 < -15) {
        return 0;
    }

    return 1;
}

static int receve_spi(void *arg)
{
    (void)arg;
    int len;
    while (1) {
        if (!pro) {
            len = receive_data_from_mcu(skfd, &u_info_g);
            if (len > 0) {
                if ((u_info_g.msg[0] << 8 | u_info_g.msg[1]) != 0xB0) {
                    continue;
                }
            }
            data = u_info_g.msg[3];
            pro = 1;
            // printf("rece data\n");
        }
    }

    return 0;
}

static int process_data(void *arg)
{
    unsigned char mic;
    unsigned char udisk;
    unsigned char music;
    DIR *dirp = NULL;
    struct dirent *direntp = NULL;
    char *substr = NULL;
    int play_flag = 0;
    int status;
    int fd;
    int ret;
    send_buf[0] = 0x00;
    send_buf[1] = 0x40;
    send_buf[2] = 0x08;
    double dB = 0;

    while (1) {
        // pthread_cond_wait(&g_cond, &g_mutex);
        // get_head(&mcu_data_quene, &temp_data);
        usleep(50);
        if (pro) {
            mic = data & 0x01;
            udisk = (data & 0x02) >> 1;
            music = (data & 0x18) >> 3;
            // printf("process_data, data:%d, mic:%d, udisk:%d, music:%d\n", data, mic, udisk, music);

            if (mic > 0) {
                // traverseQueue(&mic1_queue);
                // printf("mic is ok:%d\n", calue_mic_data());
                if (calue_mic_data()) {
                    send_buf[3] = 0x00;
                } else {
                    send_buf[3] = 0x01;
                }
                send_data_to_mcu(skfd, nlh, send_buf);
                pro = 0;
            } else if (udisk > 0) {
                dirp = opendir(STORGE_PATH);
                if (dirp != NULL) {
                    while ((direntp = readdir(dirp)) != NULL) {
                        if ((substr = strstr(direntp->d_name, UCARD_PREFIX)) != NULL) {
                            //  printf("u card has connected\n");
                            send_buf[3] = 0x02;
                            send_data_to_mcu(skfd, nlh, send_buf);
                            snprintf(wav_path, sizeof(wav_path), "%s%s/%s", STORGE_PATH, direntp->d_name, MUSIC_NAME);
                            printf("%s\n", wav_path);
                            break;
                        }
                    }

                    if (!substr) {
                        //    printf("u card has not connected\n");
                        send_buf[3] = 0x00;
                        send_data_to_mcu(skfd, nlh, send_buf);
                    }
                }
                pro = 0;
            } else if (music > 0) {
                switch (music) {
                case MUSIC_PLAY:
                    if (!play_flag) {
                        snprintf(shell_cmd, sizeof(shell_cmd), "tinyplay %s -D 0 -d 0", wav_path);
                        pid = fork();
                        if (pid == 0) {
                            // 子进程
                            execl("/bin/sh", "sh", "-c", shell_cmd, (char *)NULL);
                            exit(0); // 如果exec失败，立即退出
                        } else if (pid > 0) {
                            // 父进程
                            do {
                                // 使用非阻塞方式检查子进程状态
                                if (waitpid(pid, &status, WNOHANG) == 0) {
                                    send_buf[3] = 0x08;
                                    send_data_to_mcu(skfd, nlh, send_buf);
                                    // break;
                                    // de_quene(&mcu_data_quene);
                                }
                            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                        } else {
                            // fork 失败
                            perror("fork failed");
                        }
                        play_flag = !play_flag;
                    }
                    break;
                case MUSIC_STOP:
                    if (play_flag) {
                        kill(pid, SIGTERM);
                        send_buf[3] = 0x00;
                        send_data_to_mcu(skfd, nlh, send_buf);
                        play_flag = !play_flag;
                    }
                    break;
                default:
                    break;
                }
                pro = 0;
            }
        } else {
            continue;
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    unsigned char buff[11] = {0};
    /* saddr 表示源端口地址，daddr表示目的端口地址 */

    // char umsg[11] = "hello mcu!";
    /* 创建NETLINK socket */
    skfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_TEST);
    if (skfd == -1) {
        perror("create socket error\n");
        return -1;
    }
    struct sockaddr_nl saddr;
    memset(&saddr, 0, sizeof(saddr));
    // AF_NETLINK
    saddr.nl_family = AF_NETLINK;
    // 端口号(port ID)
    saddr.nl_pid = 110;
    saddr.nl_groups = 0;
    if (bind(skfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0) {
        perror("bind() error\n");
        close(skfd);
        return -1;
    }

    // 发送函数
    // 接收函数

    // nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PLOAD));
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(send_buf)));
    memset(nlh, 0, sizeof(struct nlmsghdr));
    // nlh->nlmsg_len = NLMSG_SPACE(MAX_PLOAD);
    // nlh->nlmsg_len = NLMSG_LENGTH(MAX_PLOAD);
    nlh->nlmsg_len = NLMSG_LENGTH(sizeof(send_buf));
    printf("nlh->nlmsg_len: %d\r\n", nlh->nlmsg_len);
    nlh->nlmsg_flags = 0;
    nlh->nlmsg_type = 0;
    nlh->nlmsg_seq = 0;
    // self port
    nlh->nlmsg_pid = saddr.nl_pid;
    // 拷贝发送的数据到报文头指向内存后面
    // memcpy(NLMSG_DATA(nlh), umsg, strlen(umsg));
    pthread_t rec_pid, rec_process, rec_play;
    int ret;
    pthread_mutex_init(&g_mutex, NULL);
    pthread_cond_init(&g_cond, NULL);
    initQueue(&mic1_queue, 10);
    initQueue(&mic2_queue, 10);
    initQueue(&mic3_queue, 10);
    initQueue(&mic4_queue, 10);
    pthread_mutex_init(&g_mutex_play, NULL);
    pthread_cond_init(&g_cond_play, NULL);
    ret = pthread_create(&rec_pid, NULL, (void *)receve_spi, NULL);
    ret = pthread_create(&rec_pid, NULL, (void *)monitor_mic, NULL);
    ret = pthread_create(&rec_process, NULL, (void *)process_data, NULL);
    if (ret == 0) {
        pthread_join(rec_process, NULL);
    } else
        return -1;

    return 0;
}
