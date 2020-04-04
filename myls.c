#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

int print_file_item (const char *file_name_s){
    char flags_s[11], *flags_sp = flags_s;
    struct stat st;
    mode_t owner, group, other;
    uid_t   uid;
    gid_t   gid;
    off_t   size;
    // TODO make dynamicly allocated
    char size_s[16], uid_s[16], gid_s[16], uname_s[32], ugr_s[32], nlink_s[16], mtime_s[32];
    struct passwd *pwd = NULL;
    struct group *ugr = NULL;

    if(stat(file_name_s, &st) < 0)
    {
        printf("Couldn't open file.\n");
        return 1;
    }

    if(S_ISREG(st.st_mode))
        *flags_sp++ = '-';
    else if(S_ISDIR(st.st_mode))
        *flags_sp++ = 'd';
    else if(S_ISCHR(st.st_mode))
        *flags_sp++ = 'c';
    else if(S_ISBLK(st.st_mode))
        *flags_sp++ = 'b';
    else if(S_ISFIFO(st.st_mode))
        *flags_sp++ = 'f';
    else if(S_ISLNK(st.st_mode))
        *flags_sp++ = 'l';
    else
        *flags_sp++ = 's';

    owner = st.st_mode & S_IRWXU;
    group = st.st_mode & S_IRWXG;
    other = st.st_mode & S_IRWXO;

    *flags_sp++ = owner & S_IRUSR ? 'r' : '-';
    *flags_sp++ = owner & S_IWUSR ? 'w' : '-';
    *flags_sp++ = owner & S_IXUSR ? 'x' : '-';

    *flags_sp++ = group & S_IRGRP ? 'r' : '-';
    *flags_sp++ = group & S_IWGRP ? 'w' : '-';
    *flags_sp++ = group & S_IXGRP ? 'x' : '-';

    *flags_sp++ = other & S_IROTH ? 'r' : '-';
    *flags_sp++ = other & S_IWOTH ? 'w' : '-';
    *flags_sp++ = other & S_IXOTH ? 'x' : '-';

    *flags_sp = '\0';

    sprintf(nlink_s, "%ld", (unsigned long int)(st.st_nlink));    

    if (!strftime(mtime_s, sizeof mtime_s, "%b %e %H:%M", localtime (&(st.st_mtime))))
        puts("strftime failed\n");    

    sprintf(size_s, "%ld", (unsigned long int)(st.st_size));
    sprintf(uid_s, "%d", (unsigned int)(st.st_uid));
    sprintf(gid_s, "%d", (unsigned int)(st.st_gid));

    pwd = getpwuid (st.st_uid);
    if (NULL != pwd)
        sprintf(uname_s, "%s", pwd->pw_name);

    ugr = getgrgid(st.st_gid);
    if (NULL != ugr)
        sprintf(ugr_s, "%s", ugr->gr_name);

    printf("%s %s %s %s %s %s %s\n", flags_s, nlink_s, uname_s, ugr_s, size_s, mtime_s, file_name_s);  

    return 0;
}

int main(void)
{
 
    // TODO make dynamicly allocated
    char current_dir_s[256];
    DIR *dir_stream;
    struct dirent *dir;

    strcpy(current_dir_s, "./");

    dir_stream = opendir(current_dir_s);
    while (1){
        dir = readdir(dir_stream);
        if (NULL!= dir){
            //printf("file = %s\n", dir->d_name);
            print_file_item(dir->d_name);
        } else
            break;
    }

/*
    entry.flagsget_flags()
    get_owner_name();
    get_file_group();
    get_size();
    get_mod_time();
    get_file_name();
    format
    free_x();
*/

  return 0;
}

