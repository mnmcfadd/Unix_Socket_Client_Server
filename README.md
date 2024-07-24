# CSE 109 - Systems Software - Final Exam

**Due Date: 5/15/2021 EOD**

## Ethics Contract

**FIRST**: Please read the following carefully:

- I have not received, I have not given, nor will I give or receive, any assistance to another student taking this exam, including discussing the exam with students in another section of the course. Do not discuss the exam after you are finished until final grades are submitted.
- If I use any resources (including text books and online references and websites), I will cite them in my assignment.
- I will not ask question about how to debug my exam code on Piazza or any other platform.
- I will not plagiarize someone else's work and turn it in as my own. If I use someone else's work in this exam, I will cite that work. Failure to cite work I used is plagiarism.
- I understand that acts of academic dishonesty may be penalized to the full extent allowed by the [Lehigh University Code of Conduct][0], including receiving a failing grade for the course. I recognize that I am responsible for understanding the provisions of the Lehigh University Code of Conduct as they relate to this academic exercise.

If you agree with the above, type your full name in the following space along with the date. Your exam **will not be graded** without this assent. When you are done, **make your first commit with the commit message: `I, <your full name here>, agree to the ethics contract`.**

Write your name and date between the lines below

---------------------------------------------
I, Max McFadden, agree to the ethics contract
---------------------------------------------

## Exam Prelude

**Read thoroughly before starting your exam:**

### Instructions 

1. Fork this repository into your CSE109 project namespace. [Instructions](https://docs.gitlab.com/ee/workflow/forking_workflow.html#creating-a-fork)
2. Clone your newly forked repository onto your development machine. [Instructions](https://docs.gitlab.com/ee/gitlab-basics/start-using-git.html#clone-a-repository) 
3. As you are writing code you should commit patches along the way. *i.e.* don't just submit all your code in one big commit when you're all done. Commit your progress as you work. **You should make at least one commit per question.**
4. When you've committed all of your work, there's nothing left to do to submit the assignment.

### Commit Policy

This exam is divided into discrete "questions", and you must make at least one commit per question. This is how I will know you are not just copying and pasting an entire solution from the internet. I will be able to see how much time you spend on each question based on your commit times. Red flags are raised when certain patterns of behavior indicating cheating are exhibited by your commit activity. 

You can do the questions in any order, and you can go back to questions for which you have already made a commit, but I want you to have at least one commit per question. The commit message for each question should be "QUESTION N" where N is the number of the question.

### Resource Usage Policy

You are free to use any resource for this exam. This includes books, notes, lecture videos, documentation, the internet, Stack Overflow, etc. The only course resources off limits are me and your TAs. I am free to answer clarifying questions, but that's it. I can't help you debug or provide any technical support (this includes support with git. Knowing how to use git is part of this exam.)

If you use any resources from the internet or anywhere else, **YOU HAVE TO CITE THEM** in your exam. Failure to cite resources you used may lead to failure of the exam. This includes the wiki article and videos I posted above. If you use them, cite them every time you use them. See the Works Cited section at the bottom for more details on how to cite your work in this document using Markdown.

## Assignment

As this is your final exam, I expect that you have acquired the necessary skills to complete it. Therefore, I will not tell you what to do step-by-step. You have a lot of latitude to complete this project as you see fit. This means you can use whichever language you want (either C or C++), you can lay out your project directory however you want, and you can use whatever tools you want (gcc, g++, clang, Make, etc.). Do whatever works for you in order to achieve the intended result.

For this exam, you will be writing the second half of a client/server pair that will communicate over Unix sockets. The pair is:

1. File Server - A file server is a program that hosts files for clients. It receives files that clients want to store, and sends them back to the client (or other clients) when they are requested. This is what you will be writing for this assignment. An example file server has been provided for you in the Homework 9 repository. You can use it as a reference for this exam.
2. Client - This program connects to the server and can send files to it, which will be stored on the file server. The client can also request files from the file server. An example client implementation has already been provided for you in Homework 9. You may use any of your work from Homework 9 for this exam, but if you do so, cite it.

## Part 2 - File Server

There are three cuts to this exam. You can choose the one you wish to complete based on how ambitious you are. The harder the cut, the higher the maximum grade you can earn. You should specify in the cut you chose at the beginning of your oral explanation.

No matter which version you choose, the file server program should accept the following flag:

1. `--hostname address:port` - Where `address` is a 32 bit IP address, and `port` is the desired port of the file server.

If you call the program without a `--hostname`, it should exit with an error.

---

### Cut 1 - Single Serve

The maximum grade for this cut is a **`B (85%)`**.

- Q1-1. The server will start, bind a socket to a port, and listen for a connection.
- When an connection is established, it will attempt to read a message from the client. The message will either be a `File` message or a `Request` message.
- The server will service the message in the following steps:
  - Q1-2. Read the message to a buffer
  - Q1-3. Decrypt the message
  - Q1-4 Deserialize the message to the appropriate struct, either a `File` or `Request`.
    - Q1-5. If the message is a `File`, then the server will save the file to disk in a folder called `received`.
    - Q1-6. If the message is a `Request`, then the server will look for the requested file in the `received` folder. 
      - If the requested file does not exist, nothing will be sent back to the client.
      - Q1-7. If the file does exist it will be read into memory, serialized into a `File` message, encrypted, and sent to the client.
    - After servicing this one message, the file server will exit.
    - If you want to send or request another file from the file server, you must restart the file server so it can service another request.

---

### Cut 2 - Sustained Connection

The maximum grade for this cut is an **`A (93%)`**.

- Q2-1. The server will start, bind a socket to a port, and listen for a connection.
- Q2-2. The server will create an empty hash map to store received files in memory.
  - You have to write the hash map yourself. You can't use a library that you did not write. You can use your Homework 6 code or the posted solution as a starting point. Remember to cite this code if you use it. If you look at any source for help, be sure to cite it.
  - The hash map will use the filename string as a key, and the file as a value (this can be either the received `File`, or the file data `vector<u8>`).
  - The hashmap will act much like the hash set, except it will have two additional methods:
    - Q2-3. `bool insert(String key, File value);` - inserts the k/v pair into the hash map. Returns true if the key already existed, and replaces the stored value with the supplied value. Returns false if the key did not exist already.
    - Q2-4. `File get(String key);` - Returns the file associated with the supplied key. If the key doesn't exist, this function throws an exception.
- When the server receives a connection, it will enter an infinite loop.
  - This loop will attempt to receive data from the client. When it receives a message, it will follow the following steps:
    - Q2-5. Read the message to a buffer
    - Q2-6. Decrypt the message
    - Q2-7. Deserialize the message to the appropriate struct, either a `File` or `Request`.
      - Q2-8. If the message is a `File`, then the server will insert the filename and file into the hash map.
      - Q2-9.If the message is a `Request`, then the server will look for the requested file in hash map. 
        - If the requested file does not exist, nothing will be sent back to the client.
        - Q2-10. If the file does exist it will be serialized into a `File` message, encrypted, and sent to the client.
  - After servicing this message, the file server will loop and wait for a new message from the client.
  - The file server will not terminate until the user terminates the program or the client terminates the connection.
  - To properly test this cut of the file server, you will have to modify your client to be able to send multiple messages without terminating the connection.

---

### Cut 3 - Multiple Connections

The maximum grade for this cut is an **`A+ (100%)`**.

- Q3-1. The server will start, bind a socket to a port, and listen for a connection.
- Q3-2. The server will create an empty hash map to store received files in memory. See the previous section for instructions on how to do that.
- The server will then enter an infinite loop and wait for new connections.
- Q3-3. When the server receives a connection, it will fork a new process and attempt to receive a message. When the child process receives a message, it will follow the following steps:
  - Q3-4. Read the message to a buffer
  - Q3-5. Decrypt the message
  - Q2-6. Deserialize the message to the appropriate struct, either a `File` or `Request`.
    - Q3-7. If the message is a `File`, then the server will insert the filename and file into the hash map.
    - Q3-8. If the message is a `Request`, then the server will look for the requested file in hash map. 
      - If the requested file does not exist, nothing will be sent back to the client.
      - Q3-9. If the file does exist it will be serialized into a `File` message, encrypted, and sent to the client.
  - When the message is sent, the file server should terminate the connection with the client
- Meanwhile, the parent process will be listening for new connections. When a new connection comes in, it will fork a new process and service that connection as laid out above.
- Q3-10. Here is the difficulty with this cut of the problem. When a new process is forked, it loses contact with the parent process. Everything that happens in the forked process will stay in that process. So the file that's received and stored in the child process will not appear in the hash map of the parent process. Therefore, if a new connection requests that file, it will be as if it has never been sent in the first place.
  - You can use a number of tools to solve this:
    - You could open a second socket between the parent and child process and send the file through that channel (this is probably the easiest method since you already have the knowledge to do this).
    - You could use shared memory (see `shmem()`).
    - You could use [boost IPC](https://www.boost.org/doc/libs/1_68_0/doc/html/interprocess.html).
    - You could use files.

## Message Protocol

### Serialized File

If you have a file called `file.txt` with the contents `Hello`, then the serialzied, unencrypted message should look like this:

```
┌───────────────────────────────────────────────────────────────────────────────┐
│0xae     // map tag                                                            │
│0x01     // 1 kv pair                                                          │
├───────────────────────────────────────────────────────┬───────────┬───────────┤
│0xaa     // string8 tag                                │           │           │
│0x04     // 4 characters                               │ key       │           │
│File     // the string "File"                          │           │           │
├───────────────────────────────────────────────────────┼───────────┤  pair 1   │
│0xae     // the value associated with the key is a map │           │           │
│0x02     // 2 kv pairs                                 │           │           │
├────────────────────────────────┬───────────┬──────────┤           │           │
│0xaa     // string8 tag         │           │          │           │           │
│0x04     // 4 characters        │ key       │          │           │           │
│name     // the string "name"   │           │          │           │           │
├────────────────────────────────┼───────────┤ pair 1   │           │           │
│0xaa     // string8 tag         │           │          │           │           │
│0x08     // 8 characters        │ value     │          │           │           │
│file.txt // the filename        │           │          │           │           │
├────────────────────────────────┼───────────┼──────────┤           │           │
│0xaa     // string8 tag         │           │          │ value     │           │
│0x05     // 5 characters        │ key       │          │           │           │
│bytes    // the string "bytes"  │           │          │           │           │
├────────────────────────────────┼───────────┤ pair 2   │           │           │
│0xac     // array8 tag          │ value     │          │           │           │
│0x05     // 5 elements          │           │          │           │           │
|0xa2     // unsigned byte tag   │           │          │           │           │
│H        // a byte              │           │          │           │           │
|0xa2     // unsigned byte tag   │           │          │           │           │
│e        // a byte              │           │          │           │           │
|0xa2     // unsigned byte tag   │           │          │           │           │
│l        // a byte              │           │          │           │           │
|0xa2     // unsigned byte tag   │           │          │           │           │
│l        // a byte              │           │          │           │           │
|0xa2     // unsigned byte tag   │           │          │           │           │
│o        // a byte              │           │          │           │           │
└────────────────────────────────┴───────────┴──────────┴───────────┴───────────┘           
```

In decimal:
```
[174, 1, 170, 4, 70, 105, 108, 101, 174, 2, 170, 4, 110, 97, 109, 101, 170, 8, 102, 105, 108, 101, 46, 116, 120, 116, 170, 5, 98, 121, 116, 101, 115, 172, 5, 162, 72, 162, 101, 162, 108, 162, 108, 162, 111]
```

### Serialized Request

If you have a file called `file.txt` that you are requesting, then the unencrypted message should look like this:

```
┌───────────────────────────────────────────────────────────────────────────────┐
│0xae     // map tag                                                            │
│0x01     // 1 kv pair                                                          │
├───────────────────────────────────────────────────────┬───────────┬───────────┤
│0xaa     // string8 tag                                │           │           │
│0x07     // 7 characters                               │ key       │           │
│Request  // the string "Request"                       │           │           │
├───────────────────────────────────────────────────────┼───────────┤  pair 1   │
│0xae     // the value associated with the key is a map │           │           │
│0x01     // 1 kv pair                                  │           │           │
├──────────────────────────────┬───────────┬────────────┤           │           │
│0xaa     // string8 tag       │           │            │           │           │
│0x04     // 4 characters      │ key       │            │ value     │           │
│name     // the string "name" │           │            │           │           │
├──────────────────────────────┼───────────┤ pair 1     │           │           │
│0xaa     // string8 tag       │           │            │           │           │
│0x08     // 8 characters      │ value     │            │           │           │
│file.txt // the filename      │           │            │           │           │
└──────────────────────────────┴───────────┴────────────┴───────────┴───────────┘           
```

In decimal:
```
[174, 1, 170, 7, 82, 101, 113, 117, 101, 115, 116, 174, 1, 170, 4, 110, 97, 109, 101, 170, 8, 102, 105, 108, 101, 46, 116, 120, 116]
```

## Video Explanation

This is the oral portion of the exam. You will record an explanation for your file server, and demonstrate that it works with the client that you wrote. To demonstrate it working, it's sufficient to show the client sending a file (any file) to the server, the server acknowledging its receipt, and then to show the client requesting it again and saving it to disk. If you did not successfully write the client, you may demonstrate your file server working with the example client binary.

If you didn't get either the file server or client working, explain how you attempted to solve this exam and where you got stuck. Show off any code you did write. This will get you full credit for this portion.

You can use Zoom to do this, [here is a link][3] to some instructions. You don't have to record your face, only your voice and the screen. Go through your code and explain how you the important parts (important is subjective here. Usually the important bits are the ones you spent the most time on or had the most difficulty with). Your goal with this section is to convince me you know what you are talking about, so I want you to do this without reading a script or written answer. When you are done, upload your recording to your final exam repository.

Recording Link(s):

## Works Cited

Include a list of websites and resources you used to complete this exam. Make a numbered list, and at the place where you used this resource cite it using [reference style links in Markdown][1] (See this Readme's source for an example. It's invisible in the rendered document, but there are a number of links below this paragraph).

[Q1-1, Basic Server Layout from HW8][4]


[0]: https://studentaffairs.lehigh.edu/content/code-conduct
[1]: https://www.markdownguide.org/basic-syntax#reference-style-links
[2]: http://crasseux.com/books/ctutorial/Building-a-library.html#Building%20a%20library
[3]: https://support.zoom.us/hc/en-us/articles/201362473-Local-recording
[4]: https://www.tutorialspoint.com/unix_sockets/socket_quick_guide.htm
