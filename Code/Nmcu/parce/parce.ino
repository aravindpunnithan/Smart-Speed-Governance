
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<curl.h>
 
static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
 
void main(int argc, char *argv[])
{
  CURL *curl_handle;
  static const char *pagefilename = "out.txt";
  FILE *pagefile;
  double lat=76.61748;
  double lng=9.31481;
  char lat_min[20],lat_max[20],lng_min[20],lng_max[20],my_url[150]="http://www.overpass-api.de/api/xapi?*[maxspeed=*][bbox=";

  snprintf(lat_min,20, "%f",lat-0.000009);
  snprintf(lat_max,20, "%f",lat+0.000009);
  snprintf(lng_min,20, "%f",lng-0.000009);
  snprintf(lng_max,20, "%f",lng+0.000009);

  strcat(my_url,lat_min);
  strcat(my_url,",");
  strcat(my_url,lng_min);
  strcat(my_url,",");
  strcat(my_url,lat_max);
  strcat(my_url,",");
  strcat(my_url,lng_max);
  strcat(my_url,"]");

//   printf("%s\n",my_url);
//   printf("http://www.overpass-api.de/api/xapi?*[maxspeed=*][bbox=76.61711,9.31695,76.61729,9.31713]\n");
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* set URL to get here */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, my_url);
//   curl_easy_setopt(curl_handle, CURLOPT_URL,"http://www.overpass-api.de/api/xapi?*[maxspeed=*][bbox=76.616341‬,9.316831,76.616359,9.316849‬]");

  /* Switch on full protocol/debug output while testing */ 
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
 
  /* disable progress meter, set to 0L to enable and disable debug output */ 
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
  /* open the file */ 
  pagefile = fopen(pagefilename, "wb");
  if(pagefile) {
 
    /* write the page body to this file handle */ 
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
 
    /* get it! */ 
    curl_easy_perform(curl_handle);
 
    /* close the header file */ 
    fclose(pagefile);
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  curl_global_cleanup();
 
  return;
}
