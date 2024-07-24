#include <pack109.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <cstring>

#define KEY 42

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[65000]; //using 65k buffer
   struct sockaddr_in serv_addr, cli_addr;
   int  n;

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));

   //checking and parsing --hostname flag
   if(argc != 3 || strcmp(argv[1],"--hostname") != 0){
       printf("Flag not passed correctly. Enter --hostname address:port");
       exit(0);
   } //checking flag
   char* nameAddr = strtok(argv[2],":");
   int i = 0;
   string nmAd[2];
   while (nameAddr != NULL) {
        nmAd[i++] = nameAddr;
        //std::cout << nmAd[i-1] << "\n";
        nameAddr = strtok(NULL, ":");
    }

   portno = stoi(nmAd[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr(nmAd[0].data());
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);
   printf("listening\n");
   clilen = sizeof(cli_addr);
   
   /* Accept actual connection from the client */
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *) &clilen);
	
   if (newsockfd < 0) {
      perror("ERROR on accept");
      exit(1);
   }
   
   /* If connection is established then start communicating */
   bzero(buffer,65000);
   n = read(newsockfd,buffer,65000 );
   if (n < 0) {
      perror("ERROR reading from socket");
      exit(1);
   }

   //decrypting and showing bytes in message
   std::vector<u8> byteVec(buffer, buffer + strlen(buffer));
   //printf("Encrypted Message: [");
    for(int i = 0; i < byteVec.size(); i++){
        //printf("%d, ",byteVec.at(i));
        byteVec.at(i) = byteVec.at(i) ^ KEY;
    }
    /*printf("]\n");
    printf("Decrypted Message: [");
    for(int i = 0; i < byteVec.size(); i++){
        printf("%d, ",byteVec.at(i));
    }
    printf("]\n");*/

   //deserializing into file struct
   if(byteVec.at(4) == 'F'){
      pack109::File file = pack109::deserialize_file(byteVec);
      std::ofstream myFile("received/" + file.name);
      printf("Message Received - Bytes: %d\nWriting to file...\n",file.bytes.size());
      myFile << file.bytes.data();
      myFile.close();
      /*for(int i = 0; i < file.bytes.size(); i++){
      printf("%c",file.bytes.at(i));
      }
      printf("\n");*/
   }else if(byteVec.at(4) == 'R'){//deserializing into request struct
      pack109::Request request = pack109::deserialize_request(byteVec);
      string nmstr = "received/" + request.name;
      if(access(nmstr.c_str(), F_OK ) == -1){
         printf("File not Found\n");
         exit(0);
      }

      ///////////////////////////////////////////////////
      //reading, serializing, sending file back to server
      std::ifstream file(nmstr, std::ios::binary | std::ios::ate);
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
      char char_nmstr[nmstr.size()];
      strcpy(char_nmstr,nmstr.c_str());
      char* fileName = strtok(char_nmstr,"/");
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
         //printf("%d, ",bytesVector.at(i));
         bytesVector.at(i) = bytesVector.at(i) ^ KEY;
      }
      /*printf("]\n");
      printf("Encrypted Message: [");
      for(int i = 0; i < bytesVector.size(); i++){
         printf("%d, ",bytesVector.at(i));
      }
      printf("]\n");*/

      /* Send message to the client */
      n = write(sockfd, bytesVector.data(), bytesVector.size());
      
      //////////////////////////////////////

   }else{
      printf("Error Handling Data\n");
      exit(0);
   }

   /* Write a response to the client */
   n = write(newsockfd,"I got your message",18);
   
   if (n < 0) {
      perror("ERROR writing to socket");
      exit(1);
   }
      
   return 0;
}