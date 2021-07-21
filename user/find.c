#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

int
match(const char* path, const char* name){
    
    int i,j=0;

    for(i=0;name[i]!='\0';i++){
        if(name[i]!=path[j]){
            return 0;
        }
        j++;
    }
    if(path[j]!=' '){
        return 0;
    }
    return 1;
}

void
find(char *path, char *name)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);    //can't open the file.
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);   // have no stat info.
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    if(match(fmtname(path),name)){
        printf("%s\n",path);
    }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      
      if(strlen(de.name) == 1 && de.name[0] == '.')
        continue;
      if(strlen(de.name) == 2 && de.name[0] == '.' && de.name[1] == '.')
        continue;

      find(buf, name);
      //printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc <= 2)
    fprintf(2, "find: not enough params provided");
  find(argv[1], argv[2]);
  exit(0);
}
