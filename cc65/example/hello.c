#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>

bool
copy_test(char *src, char *dest)
{
    FILE *in, *out;
    bool ret = false;
    
    printf("Copying from %s to %s\n", src, dest);
    if (in = fopen(src, "r")) {
        if (out = fopen(dest, "w")) {
            ret = true;
            for (;;) {
                int c = fgetc(in);
                if (c < 0) {
                    break;
                }
                fputc(c, out);
            }
            fclose(out);
        }
        fclose(in);
    }
    
    return ret;
}

bool
read_test(char *src)
{
    FILE *fp;
   
    fp = fopen(src, "r");
    if (fp != NULL) {
        printf("Reading; fp @ %p:", fp);
        for(;;) {
            int c = fgetc(fp);
            if (c < 0) {
                break;
            }
            putchar(c);
        }
        fclose(fp);
        return true;
    }

    return false;

}    

bool
write_test(char *dest)
{
    FILE *fp;
    
    printf("Writing test.txt.\n");
    fp = fopen(dest, "w");
    if (fp != NULL) {
        fwrite("test!", 1, 5, fp);
        fclose(fp);
        return true;
    }
    
    return false;
}

void 
dir(const char *path)
{
    struct DIR *dir;
    
    if (dir = opendir(path)) {
        struct dirent *dirent;
        puts("Dir Open Succeeded");
        for (;;) {
            if (dirent = readdir(dir)) {
                
                if (_DE_ISREG(dirent->d_type)) {
                    printf("        %6d  %s\n", dirent->d_blocks, dirent->d_name);
                    continue;
                }
                
                if (_DE_ISDIR(dirent->d_type)) {
                    printf("   <DIR>        %s\n", dirent->d_name);
                    continue;
                }
                
                if (_DE_ISLBL(dirent->d_type)) {
                    printf("Directory of %s\n", dirent->d_name);
                    continue;
                }
            }
            break;
        }
        closedir(dir);
    }
} 

int 
main()
{
    char c = 0;
    
    putchar(12);  // cls
    puts("Hello world!");
    puts("Waiting for IEC init to complete.");
    
    do {
        
        dir("0:");
        
        if (!write_test("0:test.txt")) {
            printf("Write test failed.\n");
            break;
        }

        if (!read_test("0:test.txt")) {
            printf("Read test failed.\n");
            break;
        }

        if (!copy_test("test.txt", "test2.txt")) {
            printf("Copy test failed.\n");
            break;
        }
        
        if (rename("test2.txt", "copy.txt") != 0) {
            printf("rename failed.\n");
            break;
        }
        
        if (!read_test("copy.txt")) {
            printf("Read test failed.\n");
            break;
        }
        
        
        if (remove("copy.txt") != 0) {
            printf("delete failed.\n");
            break;
        }
        
        dir("1:");

    } while (false);
    
    puts("");
    printf("Testing getchar(); press CTRL-C to exit.\n");
    while (c != 3) {
        c = getchar();
        putchar(c);
    }
    
    return 0;
}
