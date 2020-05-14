#include<stdio.h>
#include<string.h>
#include "gps.h"

char gps_rx_data[] = ""
"$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\r\n"
"$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A\r\n"
"$GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75\r\n"
"$GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1*39\r\n";

gps_frame_t gps_frame;

void process_gps_sentences(){
    char *p = strtok(gps_rx_data, "\r\n");

    while(p!=NULL){

        printf("%s\n", p);
        memset(&gps_frame, 0, sizeof(gps_frame));

        process_gps_data(p, &gps_frame);
        if(get_checksum(p) == cal_checksum(p))
            parse_gps_sentence(&gps_frame);
        else
            printf("checksum error\n");
        p = strtok(NULL, "\r\n");
        printf("------------------------------------------------------------------\n\n");
    }

}
int main(){


    process_gps_sentences();

    return 0;
}
