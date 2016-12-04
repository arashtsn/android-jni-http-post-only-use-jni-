
#include <jni.h>

#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>



#define SA      struct sockaddr
#define MAXLINE 4096
#define MAXSUB  200


#define LISTENQ         1024

extern int h_errno;


JNIEXPORT jstring JNICALL Java_com_example_hellojni_MainActivity_stringFromJNI( JNIEnv* env, jobject thiz )
{


    struct in_addr ipv4addr;
    struct in6_addr ipv6addr;
	int sockfd;
	struct sockaddr_in servaddr;

	char **pptr;
	//********** You can change. Puy any values here *******
	char *hname = "androidexample.com";
	char *page = "/media/webservice/httppost.php";
	char *poststr = "name=login&&email=test@g.com&&user=test&&pass=12345";
	//*******************************************************

	char str[50];
	struct hostent *hptr;
        inet_pton(AF_INET, "67.20.76.109", &ipv4addr);
        hptr = gethostbyaddr((char *)&ipv4addr, sizeof ipv4addr, AF_INET);
	if (hptr == NULL) {
		fprintf(stderr, " gethostbyaddr error for host: %s: %s",
			hname, hstrerror(h_errno));
		//exit(1);
          return  (*env)->NewStringUTF(env, "gethostbyaddr!");
	}
	//printf("hostname =: %s\n",
	hptr->h_name;
	if (hptr->h_addrtype == AF_INET
	    && (pptr = hptr->h_addr_list) != NULL) {
		//printf("address: %s\n",
		       inet_ntop(hptr->h_addrtype, *pptr, str,
				 sizeof(str));
	} else {
		fprintf(stderr, "Error call inet_ntop \n");
       return  (*env)->NewStringUTF(env, "Error call inet_ntop ");
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	inet_pton(AF_INET, str, &servaddr.sin_addr);
      connect(sockfd, (SA *) & servaddr, sizeof(servaddr));
	
        char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
	ssize_t n;
	snprintf(sendline, MAXSUB,
		 "POST %s HTTP/1.0\r\n"
		 "Host: %s\r\n"
		 "Content-type: application/x-www-form-urlencoded\r\n"
		 "Content-length: %d\r\n\r\n"
		 "%s", page, hname, strlen(poststr), poststr);


	write(sockfd, sendline, strlen(sendline));

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {

		recvline[n] = '\0';
		//printf("%s", recvline);
	}

	close(sockfd);

FILE *f = fopen("/sdcard/hello.txt", "w+");
fwrite(recvline, sizeof(recvline), sizeof(recvline), f);
fclose(f);

  return  (*env)->NewStringUTF(env, recvline);
}





int sendmessage(jstring js, jobject thiz,JNIEnv* env )
{
 


 jclass MainActivity= (*env)->FindClass(env,"com/example/hellojni/MainActivity");
 jmethodID  logMessageDirect = (*env)->GetMethodID(env,MainActivity,"logMessageDirect","(Ljava/lang/String;)V");
 (*env)->CallVoidMethod(env,thiz,logMessageDirect,js);
return 0;
}
