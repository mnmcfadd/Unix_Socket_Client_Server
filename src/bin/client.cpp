#include <pack109.hpp>
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#define KEY 42

int main(int argc, char *argv[]) {
   int sockfd, portno, n;
   struct sockaddr_in serv_addr;
   struct hostent *server;
   
   char buffer[65000];
   
   if (argc < 3) {
      fprintf(stderr,"usage %s hostname port\n", argv[0]);
      exit(0);
   }

   //parsing and checking --hostname flag
	if(argc != 5 || strcmp(argv[1],"--hostname") != 0){
       printf("Flag not passed correctly. Enter --hostname address:port then --send or --request filename");
       exit(0);
   } //checking flag
   char* nameAddr = strtok(argv[2],":");
   int i = 0;
   string nmAd[2];
   while (nameAddr != NULL) {
        nmAd[i++] = nameAddr;
        nameAddr = strtok(NULL, ":");
   }
   
   portno = atoi(nmAd[1].data());
   
   /* Create a socket point */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
	
   server = gethostbyname(nmAd[0].data());

   if (server == NULL) {
      fprintf(stderr,"ERROR, no such host\n");
      exit(0);
   }
   
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(portno);
   
   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR connecting");
      exit(1);
   }

   //checking which flag was called
   if(strcmp(argv[3],"--send") == 0){
      //reading from file to buffer, putting buffer onto vector<u8>
      std::ifstream file(argv[4], std::ios::binary | std::ios::ate);
      if(!file){
         printf("File not found\n");
         exit(0);
      }
      std::streamsize size = file.tellg();
      file.seekg(0, std::ios::beg);

      //std::vector<char> buffer(size);
      bzero(buffer,65000);
      file.read(buffer, size);
      std::vector<u8> byteVec(buffer, buffer + strlen(buffer));
      
      //vec serByteVec = pack109::serialize(byteVec);

      //parsing filename to remove path
      char* fileName = strtok(argv[4],"/");
      std::vector<string> fnArr;
      while (fileName != NULL) {
         fnArr.push_back(fileName);
         fileName = strtok(NULL, "/");
      }
      //initializing File struct
      pack109::File item;
      item.name = fnArr.back();
      item.bytes = byteVec;

      vec bytesVector = pack109::serialize(item);
      //encrypting bytes
      //printf("Message: [");
      for(int i = 0; i < bytesVector.size(); i++){
        // printf("%d, ",bytesVector.at(i));
         bytesVector.at(i) = bytesVector.at(i) ^ KEY;
      }
      /*printf("]\n");
      printf("Encrypted Message: [");
      for(int i = 0; i < bytesVector.size(); i++){
         printf("%d, ",bytesVector.at(i));
      }
      printf("]\n");*/

      /* Send message to the server */
      n = write(sockfd, bytesVector.data(), bytesVector.size());
      
      if (n < 0) {
         perror("ERROR writing to socket");
         exit(1);
      }
   }else if(strcmp(argv[3],"--request") == 0){
      pack109::Request item;
      item.name = argv[4];
      vec bytesVector = pack109::serialize(item);
      //printf("Message: [");
      for(int i = 0; i < bytesVector.size(); i++){
         //printf("%d, ",bytesVector.at(i));
         bytesVector.at(i) = bytesVector.at(i) ^ KEY;
      }
      /*printf("]\n");
      printf("Encrypted Message: [");
      for(int i = 0; i < bytesVector.size(); i++){
         printf("%d, ",bytesVector.at(i));
      }
      printf("]\n");*/

      /* Send message to the server */
      n = write(sockfd, bytesVector.data(), bytesVector.size());

         /* Now read server response */
   }else{
      printf("Flag not passed correctly. Enter --send filename or --request filename after --hostname address:port");
      exit(0);
   }

   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
   
   /* Now read server response */
   bzero(buffer,65000);
   n = read(sockfd, buffer, 65000);

   
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }
	
   printf("%s\n",buffer);
   return 0;
}