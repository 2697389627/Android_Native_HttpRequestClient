#include <jni.h>
#include <string>

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IPSTR "42.121.252.58"
#define PORT 80
#define BUFSIZE 1024

#include <android/log.h>
#define LOG_TAG    "webronin_26"
#define LOGD(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_webronin_126_android_1native_1httprequestclient_MainActivity_NativeSendRequest(JNIEnv *env, jobject instance) {

    int sockfd, ret, i, h;
    struct sockaddr_in servaddr;
    char str1[4096], str2[4096], buf[BUFSIZE], *str;
    socklen_t len;
    fd_set   t_set1;
    struct timeval  tv;

    std::string httpResponse = "ERROR";

    // 如果小於0，等於連接失敗
    if ( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        return env->NewStringUTF(httpResponse.c_str());
    };

    bzero( &servaddr, sizeof( servaddr ) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // 如果小於0，等於連接失敗
    if ( inet_pton( AF_INET, IPSTR, &servaddr.sin_addr ) <= 0 ){
        return env->NewStringUTF(httpResponse.c_str());
    };

    // 如果小於0，等於連接失敗
    if ( connect( sockfd, ( struct sockaddr * ) &servaddr, sizeof( servaddr )) < 0) {
        return env->NewStringUTF(httpResponse.c_str());
    }

    memset(str2, 0, 4096);
    strcat(str2, "theDataToPost");
    str = (char *)malloc(128);
    len = strlen(str2);
    sprintf(str, "%d", len);

    memset(str1, 0, 4096);
    strcat(str1, "GET /ymnets/p/6255674.html HTTP/1.1\n");
    strcat(str1, "Host: www.cnblogs.com\n");
    strcat(str1, "Content-Type: text/html\n");
    strcat(str1, "Content-Length: ");

    strcat(str1, str);
    strcat(str1, "\n\n");

    strcat(str1, str2);
    strcat(str1, "\r\n\r\n");

    // 這邊等於是用socket發送了多少字節碼 => str1
    ret = write( sockfd , str1 , strlen( str1 ) );

    // 小於 0 等於發送失敗
    if (ret < 0) {
        return env->NewStringUTF(httpResponse.c_str());
    }

    FD_ZERO(&t_set1);
    FD_SET(sockfd, &t_set1);

    while(1){

        sleep(2);
        tv.tv_sec= 0;
        tv.tv_usec= 0;
        h= 0;

        h= select(sockfd +1, &t_set1, NULL, NULL, &tv);

        //if (h == 0) continue;
        if (h < 0) {
            close(sockfd);
            return env->NewStringUTF(httpResponse.c_str());
        };

        if (h > 0){
            memset(buf, 0, 4096);
            i = read(sockfd, buf, 4095);
            if (i==0){
                close(sockfd);
                return env->NewStringUTF(httpResponse.c_str());
            }
            LOGD( "%s\n", buf );
        }
    }
}
