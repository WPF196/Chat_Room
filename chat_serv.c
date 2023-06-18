#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>


#define BUF_SIZE 100
#define MAX_CLNT 256

void * handle_clnt(void * arg);
void send_msg(char * msg, long long len);
void error_handling(char * msg);
char *get_current_time();

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;



int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL);
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    while (1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }
    close(serv_sock);

    return 0;
}

void *handle_clnt(void * arg)
{
    int clnt_sock = *((int *)arg);
    long long str_len = 0;
    int i;

    while (1)
    {
    	char msg[BUF_SIZE] = "";
    	if(str_len = read(clnt_sock, msg, sizeof(msg)) > 0)
    	{
        	if(strncmp(msg, "image\n", str_len) == 0)
        	{
        		send_msg("image\n", sizeof("image\n"));
        		long long image_size;
        		read(clnt_sock, &image_size, sizeof(image_size));
        		for (i = 0; i < clnt_cnt; i++)
        			write(clnt_socks[i], &image_size, sizeof(image_size));
        		long long received_size = 0;
        		char *image_buf = malloc(image_size);
        		while (received_size < image_size)
                	{
                    		str_len = read(clnt_sock, image_buf + received_size, image_size - received_size);
                    		send_msg(image_buf + received_size, str_len);
                    		received_size += str_len;
                    		
                	}
                	free(image_buf);
        	}
        	else
        	{
        		char time_msg[BUF_SIZE + 60];
       	 		sprintf(time_msg, "[%s] %s", get_current_time(),  msg);
        		send_msg(time_msg, str_len + 60);
        	}       	
        }
    }
    pthread_mutex_lock(&mutx);
    for (i = 0; i < clnt_cnt; i++)
    {
        if (clnt_sock == clnt_socks[i])
        {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);

    return NULL;

}

void send_msg(char *msg,long long len)
{
    int i;
    pthread_mutex_lock(&mutx);
    for (i = 0; i < clnt_cnt; i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);    
}

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

char *get_current_time()
{
    time_t current_time;
    char *time_string;

    current_time = time(NULL);
    if (current_time == ((time_t)-1))
    {
        fprintf(stderr, "Error during time retrieval.\n");
        exit(1);
    }

    time_string = ctime(&current_time);
    if (time_string == NULL)
    {
    	fprintf(stderr, "Error converting the current time.\n");
        exit(1);
    }

    time_string[strlen(time_string) - 1] = '\0';

    return time_string;
}
