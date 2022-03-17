#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<sys/socket.h>
#include<errno.h> 
#include<netdb.h> 
#include<arpa/inet.h>
#define BUFFERLENGTH 40

int main(int argc,char *argv[]) {

    if (argc <= 1) {
        printf("No domain given. \n");
        return 1;
    }

    struct addrinfo hints;  
    memset(&hints, 0, sizeof(hints));
    hints.ai_family    = AF_UNSPEC;
    hints.ai_socktype  = SOCK_STREAM;


    for (int i = 1; i < argc; i++) {
        struct addrinfo *ai0, *ai;
        int error;
    

        if ((error = getaddrinfo(argv[i], "80", &hints, &ai0)) != 0) {
            printf("Unable to look up IP address: %s", gai_strerror(error));
            return 2;
        }

  // Try to connect to each address in turn
        for (ai = ai0; ai != NULL; ai = ai->ai_next) {
            char buffer[BUFFERLENGTH] = {0};
      
            /*void * addr = ai->ai_family;
            if (void * addr  == AF_INET6) {
                (void *)&((struct sockaddr_ipv6*)ai->ai_addr)->sin6_addr;
            }
            else {
                (void *)&((struct sockaddr_ipv4*)ai->ai_addr)->sin_addr;
            }
            */
            void * addr = ai->ai_family == AF_INET6 
                ? (void *)&((struct sockaddr_in6*)ai->ai_addr)->sin6_addr
                : (void *)&((struct sockaddr_in*)ai->ai_addr)->sin_addr;

            if (inet_ntop(ai->ai_family, addr, buffer, sizeof(buffer)) == NULL){
                printf("IP addresses could not be converted to a String");
                return 3;
            }
        
            if (ai->ai_family == AF_INET6) {
                printf("%s %s %s\n", argv[i],"IPv6", buffer);
            }
            else {
                printf("%s %s %s\n", argv[i],"IPv4", buffer);
            }
            
        
        //printf("%s %s %s\n", argv[i], ai->ai_family == AF_INET6 ? "IPv6" : "IPv4", buffer);
        }   
    }
    return 0;
}