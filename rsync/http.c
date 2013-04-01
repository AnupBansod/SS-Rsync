/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
/* Example source code to show how the callback function can be used to
 * download data into a chunk of memory instead of storing it in a file.
 */

#include <curl/curl.h>
#include <stdio.h>
#include "rsync.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};


static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    exit(EXIT_FAILURE);
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}


void construct_http_req()
{
  CURL *curl_handle;
  CURL *curl;
  int n = 1;
  struct MemoryStruct chunk;
  CURLcode res;
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl = curl_easy_init();
  char *buf,ch;
  /* specify URL to get */
  //curl_easy_setopt(curl_handle, CURLOPT_URL, "http://192.168.16.101:8080/a.html");
   if (curl){
  int f_in, f_out;
 // f_in = STDIN_FILENO;
 // f_out = STDOUT_FILENO; 
/*while(n > 0)
{
   n = read(STDIN_FILENO, ch, 1);
   *buf = ch; 
    buf++;
 
}*/
//	close(STDIN_FILENO);
	FILE *fp;
	fp = fopen ("httpdebug.txt","a");
	fprintf(fp,"\n\n bufinread-- %s\n\n",buf);
	fclose(fp);
  curl_easy_setopt(curl, CURLOPT_URL, "192.168.16.101:8080/a.html");
    /* Now specify the POST data */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "kahi_pan");
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
       printf("POST success!!");

  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  curl_easy_perform(curl);

  /* cleanup curl stuff */
  curl_easy_cleanup(curl);

  /*
   * Now, our chunk.memory points to a memory block that is chunk.size
   * bytes big and contains the remote file.
   *
   * Do something nice with it!
   *
   * You should be aware of the fact that at this point we might have an
   * allocated data block, and nothing has yet deallocated that data. So when
   * you're done with it, you should free() it as a nice application.
   */

  printf("%lu bytes retrieved\n", (long)chunk.size);

  if(chunk.memory)
    free(chunk.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();
}
  
}
