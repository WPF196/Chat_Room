#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>

char username[100], password[100];
int sockfd = 0;
FILE *file, *image_file1, *image_file2;

void login() {
    do
    {
    	printf("Enter username: ");
    	fgets(username, sizeof(username), stdin);
    	username[strcspn(username, "\n")] = '\0';
    
    	printf("Enter password: ");
    	fgets(password, sizeof(password), stdin);
    	password[strcspn(password, "\n")] = '\0';
    	
    	if(strlen(username) == 0 || strlen(password) == 0)
    		printf("The account password cannot be empty. Please re-enter it!\n");
    }
    while (strlen(username) == 0 || strlen(password) == 0);      
}

void showMenu() {
    system("clear");
    printf("======================= Welcome to Chat Room ========================\n");
    printf("1. Login(If you are not registed,you are automatically registered)\n");
    printf("2. Quit\n");
    printf("=====================================================================\n");
}

void handleUserChoice(int choice) {
    	switch (choice) 
    	{
        	case 1:
        	    	login();
        	    	system("clear");
        	  	printf("Logged in successfully!\n");
        	 	break;
        	case 2:
        		system("clear");
        	    	printf("Goodbye!\n");
        	    	sleep(2);
        	    	system("clear");
        	    	close(sockfd);
        	    	exit(0);            
        	default:
        		system("clear");
        	    	printf("Invalid choice. Please try again.\n");
        	    	sleep(3);
        	    	break;
    	}
}

void *client_recv(void *arg)
{
    char ch;
    int len = 0;
    while (1)
    {
        char buf[1024] = "";
        printf("---------------------------message--------------------------------\n");
        if ((len = recv(sockfd, buf, sizeof(buf), 0)) > 0)
        {
            if (strncmp(buf, "image\n", len) == 0)
            {
                char command[50];
                sprintf(command, "rm image_%d.jpg -f", getpid());
                system(command);
                char filename[20];
                sprintf(filename, "image_%d.jpg", getpid());
                image_file2 = fopen(filename, "wb+");
                long long image_size;
                recv(sockfd, &image_size, sizeof(image_size), 0);
                char *image_buf = malloc(image_size);
                long long received_size = 0;
                while (received_size < image_size)
                {
                    	len = recv(sockfd, image_buf + received_size, image_size - received_size, 0);
                    	received_size += len;
                }
                if (received_size == image_size)
                {
                  	if (fwrite(image_buf, sizeof(char), image_size, image_file2) < (size_t)image_size)
                       	perror("fwrite");
                }
                free(image_buf);
                fclose(image_file2);
                if(image_size != 0)
                	printf("Received image!\n");
            }
            else 
            {
                char filename[20];
                sprintf(filename, "file_%d", getpid());
                file = fopen(filename, "a+");
                fprintf(file, "%s\n", buf);
                fflush(file);
                system("clear");
                fseek(file, 0, SEEK_SET);
                while ((ch = fgetc(file)) != EOF)
                {
                    printf("%c", ch);
                }
                fclose(file);
           }
        }
    }
}

void *client_send(void *arg)
{
    char ch;
    while (1)
    {
        char buf[924] = "";
        if (fgets(buf, sizeof(buf), stdin) != NULL)
        {
            if (strcmp(buf, "image\n") == 0)
            {
                send(sockfd, "image\n", sizeof("image\n"), 0);
                char image_name[20] = "";
                printf("image name is:");
                fgets(image_name, sizeof(image_name), stdin);
                image_name[strcspn(image_name, "\n")] = '\0';
                image_file1 = fopen(image_name, "rb");
                if (image_file1 != NULL)
                {
                    fseek(image_file1, 0, SEEK_END);
                    long long image_size = ftell(image_file1);
                    send(sockfd, &image_size, sizeof(image_size), 0);
                    rewind(image_file1);
                    char *image_buf = malloc(image_size);
                    fread(image_buf, 1, image_size, image_file1);
                    fclose(image_file1);
                    send(sockfd, image_buf, image_size, 0);
                    free(image_buf);
                }
                else
                {
                    long long image_size = 0;
                    send(sockfd, &image_size, sizeof(image_size), 0);
                    printf("image file not found\n");
                }
            }
            else if (strcmp(buf, "quit\n") == 0)
            {
                break;
            }
            else
            {
                char user_buf[1024] = "";
                strcpy(user_buf, username);
		user_buf[strcspn(user_buf, "\n")] = ':';
    		user_buf[strcspn(user_buf, ":") + 1] = ':';
                for (int i = strcspn(user_buf, ":") + 2, j = 0; j < strcspn(buf, "\n") + 1; i++, j++)
                {    
                	user_buf[i] = buf[j];
                	user_buf[i+1] = '\0';
                }
                send(sockfd, user_buf, strlen(user_buf), 0);
            }
        }
    }
    char command[50];
    sprintf(command, "rm file_%d -f", getpid());
    system(command);
    sprintf(command, "rm image_%d.jpg -f", getpid());
    system(command);
    exit(0);
}

int main(int argc, char *argv[])
{   
    if(argc != 3){
        printf("Usage: %s <IP> <Port>\n", argv[0]);
        exit(1);
    }
        int choice = 0;
        do
        {
        	showMenu();
        
        	
        	printf("Enter your choice: ");
        	scanf("%d", &choice);
        	getchar(); 
        
        	handleUserChoice(choice);
        }while(choice != 1);
        
	system("clear");
    	unsigned short port = htons(atoi(argv[2]));
    	char *server_ip = inet_addr(argv[1] );
    	pthread_t tid1, tid2;
    	int err_log = 0;
    	struct sockaddr_in server_addr;
    		
    	if (argc > 1)
        	server_ip = argv[1];
    	if (argc > 2)
        	port = atoi(argv[2]);

    	bzero(&server_addr, sizeof(server_addr));
    	server_addr.sin_family = AF_INET;
    	server_addr.sin_port = htons(port);
    	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	err_log = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    	if (err_log < 0)
    	{
        	perror("connect");
        	exit(1);
    	}
    	printf("\n===== Welcome to Chat Room =====\n");
    	printf("     User: %s\n", username);
    	printf("===============================\n\n");
    		
    	pthread_create(&tid1, NULL, client_recv, NULL);
    	pthread_create(&tid2, NULL, client_send, NULL);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

 	close(sockfd);
    	return 0;
}


