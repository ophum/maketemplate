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
      if(!strcmp(command, "time")){ // 日付を出力
        t = time(NULL);
        date = localtime(&t);
        snprintf(str, 11, "%4d/%02d/%02d", 1900 + date->tm_year, date->tm_mon + 1, date->tm_mday);
      }else if(!strcmp(command, "in")){ // 入力
        scanf("%s", str);
      }else if(!strncmp(command, "out", 3)){  // 出力 (出力結果に影響なし)
        for(j = 4; command[j] != '\0'; j++){
          putchar(command[j]);
        }
      }


      // 書き込み
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

    printf("テンプレートファイル名を入力してください:");
    scanf("%s", tempfile);

    printf("作成するファイル名を入力してください:");
    scanf("%s", makefile);

    if((fp = fopen(tempfile, "r")) == NULL){
        printf("テンプレートファイルを開けません\n");
        return 1;
    }

    // ファイルサイズ取得
    fseek(fp, 0, SEEK_END);
    tempsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    temp = (char *)calloc(sizeof(char), tempsize);  // メモリ確保
    fread(temp, sizeof(char), tempsize, fp);        // 読み込み

    fclose(fp);

    if((fp = fopen(makefile, "w")) == NULL){
        printf("ファイル作成中にエラーが発生\n");
        return 1;
    }

    make(temp, tempsize, fp);

    fclose(fp);
    return 0;
}
