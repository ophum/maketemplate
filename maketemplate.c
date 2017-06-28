#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void make(char *template, int size, FILE *fp){
  int i, j;
  char command[128];
  time_t t;
  struct tm *date;
  char str[1024];

  for(i = 0; i < size; i++){
    switch(template[i]){
    case '\\':
      switch(template[i+1]){
      case '\\': fputc('\\', fp); i++; break;
      case '$': fputc('$', fp); i++; break;
      }
      break;
    case '$':
      for(++i, j = 0; i < size; i++, j++){
        if(template[i] == '$'){
          break;
        }
        command[j] = template[i];
        command[j+1] = '\0';
      }
      str[0] = '\0';
      if(!strcmp(command, "time")){ // ���t���o��
        t = time(NULL);
        date = localtime(&t);
        snprintf(str, 11, "%4d/%02d/%02d", 1900 + date->tm_year, date->tm_mon + 1, date->tm_mday);
      }else if(!strcmp(command, "in")){ // ����
        scanf("%s", str);
      }else if(!strncmp(command, "out", 3)){  // �o�� (�o�͌��ʂɉe���Ȃ�)
        for(j = 4; command[j] != '\0'; j++){
          putchar(command[j]);
        }
      }


      // ��������
      for(j = 0; str[j] != '\0'; j++){
        fputc(str[j], fp);
      }

      break;
    default:
      fputc(template[i], fp);
      break;
    }
  }

}
int main(int argc, char **argv){
    FILE *fp;
    char tempfile[64];
    char makefile[64];
    char *temp;
    int tempsize = 0;

    printf("�e���v���[�g�t�@�C��������͂��Ă�������:");
    scanf("%s", tempfile);

    printf("�쐬����t�@�C��������͂��Ă�������:");
    scanf("%s", makefile);

    if((fp = fopen(tempfile, "r")) == NULL){
        printf("�e���v���[�g�t�@�C�����J���܂���\n");
        return 1;
    }

    // �t�@�C���T�C�Y�擾
    fseek(fp, 0, SEEK_END);
    tempsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    temp = (char *)calloc(sizeof(char), tempsize);  // �������m��
    fread(temp, sizeof(char), tempsize, fp);        // �ǂݍ���

    fclose(fp);

    if((fp = fopen(makefile, "w")) == NULL){
        printf("�t�@�C���쐬���ɃG���[������\n");
        return 1;
    }

    make(temp, tempsize, fp);

    fclose(fp);
    return 0;
}
