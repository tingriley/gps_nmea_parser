#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include"gps.h"

gps_header_t check_header(gps_frame_t *gps_frame){

    char header[20] = {0};
    strcpy(header, gps_frame->term[0].string);

    if(strstr(header, "GGA"))
        return GGA;
    if(strstr(header, "GSV"))
        return GSV;
    if(strstr(header, "RMC"))
        return RMC;
    if(strstr(header, "GSA"))
        return GSA;
    return UNKNOWN;
}

int cal_checksum(char * s){
    int crc = 0;
    for (int i = 1; i < strlen(s) - 3 && s[i]!='*'; i ++) {
        crc ^= s[i];
    }
    return crc;
}

int get_checksum(char * s){
    char checksum[5] = {0};
    int start = strlen(s)-1;
    int c = 0;
    int crc = 0;


    for (int i = 0; i < strlen(s)-1 ; i ++) {
        if(s[i] == '*'){
            start = i + 1;
            break;
        }
    }

    for (int i = start; i < strlen(s) ; i ++) {
        if(c < 4)
            checksum[c++] = s[i];

    }
    crc = (unsigned char)strtol(checksum, NULL, 16);
    return crc;
}


uint8_t parse_gga_frame(gps_frame_t * frame, gga_frame_t* gga_frame){
    gga_frame -> fix_time = atoi(frame->term[1].string);
    gga_frame -> lat = atof(frame->term[2].string);
    gga_frame -> lat_loc = frame->term[3].string[0];
    gga_frame -> lon = atof(frame->term[4].string);
    gga_frame -> lon_loc = frame->term[5].string[0];
    gga_frame -> fix_quality = atoi(frame->term[6].string);
    gga_frame -> num_sats = atoi(frame->term[7].string);
    gga_frame -> dilution = atof(frame->term[8].string);
    gga_frame -> altitude = atof(frame->term[9].string);
    gga_frame -> geoid = atof(frame->term[11].string);

    int time = gga_frame -> fix_time;
    int hh = time/10000;
    int mm = (time-hh*10000)/100;
    int ss = (time-hh*10000 - mm*100);
    printf("Fixed Time: %02d:%02d:%02d, ", hh, mm, ss);
    printf("lat: %.3f%c, lon: %.3f%c\n",
           gga_frame -> lat,
           gga_frame -> lat_loc,
           gga_frame -> lon,
           gga_frame -> lon_loc);

    printf("Fixed quality: %d, satellites: %d\ndilution: %.2f altitude: %.2f, geoid: %.2f\n",
           gga_frame -> fix_quality,
           gga_frame -> num_sats,
           gga_frame -> dilution,
           gga_frame -> altitude,
           gga_frame -> geoid);
    
    return 0;
}

uint8_t parse_rmc_frame(gps_frame_t * frame, rmc_frame_t* rmc_frame){
    rmc_frame -> fix_time = atoi(frame->term[1].string);
    rmc_frame -> status = frame->term[2].string[0];
    rmc_frame -> lat = atof(frame->term[3].string);
    rmc_frame -> lat_loc = frame->term[4].string[0];
    rmc_frame -> lon = atof(frame->term[5].string);
    rmc_frame -> lon_loc = frame->term[6].string[0];
    rmc_frame -> speed = atof(frame->term[7].string);
    rmc_frame -> angle = atof(frame->term[8].string);
    rmc_frame -> date = atoi(frame->term[9].string);
    rmc_frame -> magnetic = atof(frame->term[10].string);
    rmc_frame -> magnetic_loc = frame->term[11].string[0];
    int date = rmc_frame -> date;
    int dd = date/10000;
    int mm = (date-dd*10000)/100;
    int yy = 1900 + (date-dd*10000 - mm*100);

    int time = rmc_frame -> fix_time;
    int h = time/10000;
    int m = (time-h*10000)/100;
    int s = (time-h*10000 - m*100);
    printf("Fixed Time: %02d:%02d:%02d, ", h, m, s);

    printf("Active: %c, lat: %.3f%c, lon: %.3f%c\n",
           rmc_frame -> status,
           rmc_frame -> lat,
           rmc_frame -> lat_loc,
           rmc_frame -> lon,
           rmc_frame -> lon_loc);
    printf("speed: %.3f, angle: %.3f, date: %d:%02d:%02d, magnetic: %.1f%c\n",
           rmc_frame -> speed,
           rmc_frame -> angle,
           yy,mm,dd,
           rmc_frame -> magnetic, rmc_frame -> magnetic_loc);

    return 0;
}

uint8_t parse_gsv_frame(gps_frame_t * frame, gsv_frame_t* gsv_frame){
    gsv_frame -> num_sentence = atoi(frame->term[1].string);
    gsv_frame -> s_num = atoi(frame->term[2].string);
    gsv_frame -> num_sats = atoi(frame->term[3].string);
    for(int i = 0; i < 4; i++){
        gsv_frame -> prn[i] = atoi(frame->term[4 + i*4].string);
        gsv_frame -> elev[i] = atoi(frame->term[4 + i*4 + 1].string);
        gsv_frame -> azimuth[i] = atoi(frame->term[4 + i*4 + 2].string);
        gsv_frame -> snr[i] = atoi(frame->term[4 + i*4 + 3].string);
    }

    printf("sentences: %d sentence: %d satellites: %d\n", gsv_frame -> num_sentence, gsv_frame -> s_num, gsv_frame -> num_sats);
    printf("prn\telev\tazimuth\t\tsnr\n");
    for(int i = 0; i < 4; i++){
        printf("%02d\t%02d\t%03d\t\t%02d\n", gsv_frame -> prn[i], gsv_frame -> elev[i], gsv_frame -> azimuth[i], gsv_frame -> snr[i]);
    }

    return 0;
}



uint8_t parse_gsa_frame(gps_frame_t * frame, gsa_frame_t* gsa_frame){
    gsa_frame -> selection = frame->term[1].string[0];
    gsa_frame -> fix_type = atoi(frame->term[2].string);
    for(int i = 0; i < 12; i++){
        gsa_frame -> prn[i] = atoi(frame->term[3 + i].string);
    }
    gsa_frame -> PDOP = atof(frame->term[15].string);
    gsa_frame -> HDOP = atof(frame->term[16].string);
    gsa_frame -> VDOP = atof(frame->term[17].string);

    printf("selection: %c, fix_type: %d\n", gsa_frame -> selection, gsa_frame -> fix_type);

    printf("PRN: ");
    for(int i = 0; i < 12; i++){
        if(i!=0)
            printf(" %d", gsa_frame -> prn[i]);
        else
            printf("%d", gsa_frame -> prn[i]);
    }
    printf("\n");

    printf("PDOP: %.2f, HDOP: %.2f, VDOP: %.2f\n", gsa_frame -> PDOP, gsa_frame -> HDOP, gsa_frame -> VDOP);

    return 0;
}

/**********************************************************
 *
 * separate the string and store them in gps_frame struct
 *
***********************************************************/
void process_gps_data(char *s, gps_frame_t * frame){
    char word[20] = {0};
    int c = 0;
    int id = 0;
    int i = 0;
    for(i = 0; i < strlen(s) && s[i]!='*'; i++){
        if(s[i] == ','){
            memcpy(frame -> term[id].string, word, strlen(word));
            id ++;
            c = 0;
            memset(word, 0, sizeof(word));
        }
        else{
            if(s[i]!='\r' && s[i]!='\n')
            word[c++] = s[i];
        }
    }
    memcpy(frame -> term[id++].string, word, strlen(word));


    for(i++, c = 0; s[i]!='\r' && s[i]!='\n' && i< strlen(s);i++){
        word[c++] = s[i];
    }

    memcpy(frame -> term[id++].string, word, strlen(word));
    frame -> max_term = id;
}

/**********************************************************
 *
 * parse gps stence according to header
 *
***********************************************************/

void parse_gps_sentence(gps_frame_t *gps_frame){
    switch(check_header(gps_frame)){
    case GGA:
        {
            gga_frame_t gga_frame;
            memset(&gga_frame, 0, sizeof(gga_frame));
            parse_gga_frame(gps_frame, &gga_frame);
            break;
        }

    case RMC:
        {
            rmc_frame_t rmc_frame;
            memset(&rmc_frame, 0, sizeof(rmc_frame));
            parse_rmc_frame(gps_frame, &rmc_frame);
            break;
        }

    case GSV:
        {
            gsv_frame_t gsv_frame;
            memset(&gsv_frame, 0, sizeof(gsv_frame));
            parse_gsv_frame(gps_frame, &gsv_frame);
            break;
        }
    case GSA:
        {
            gsa_frame_t gsa_frame;
            memset(&gsa_frame, 0, sizeof(gsa_frame));
            parse_gsa_frame(gps_frame, &gsa_frame);
            break;
        }
    default:
        break;

    }

}

