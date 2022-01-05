#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_EXTENSION     "_dmp.h"
#define DEFAULT_VAR_EXTENSION "_h_dmp"
#define LEN_SUFFIX  "_len"
#define BYTES_PER_LINE 8
#define INDENT "    "

void usage(char * argv[]) {
    printf("usage: simplehexdump <input_file> [output_file] [output_var_name]\n");
}

void strcaseup(char* str){
    //Makes all characters of a string UPPERCASE
    if(str[0] == EOF) return;

    int i = 0;
    char c;
    do{
        c = str[i];
        if(c >= 'a' && c <= 'z') {
            str[i] = c + ('A'-'a');
        }
        i++;
    }while (c != '\0');
}


void copy_and_replace(char** out, char* in, char sep, char* end) {
    //Allocates out,
    //Copies in into out, stopping the copy when encountering sep,
    //Finally, appends end to out
    
    *out = malloc(sizeof(char) * strlen(in) + strlen(end) + 1); //strlen doesnt count terminal \0
    int i = 0;

    while (in[i] != '\0') {
        // Copies in into out, stopping at sep
        (*out)[i] = in[i];
        if (in[i] == sep) {
            break;  //Stop at sep
        }
        i++;
    }

    //Copies end into out
    strcpy(*out + i, end);
    /*for (int j = 0; j < strlen(end); j++) {
        (*out)[i+j] = end[j];
    }*/

    (*out)[i + strlen(end)] = '\0';
}


int main(int argc, char * argv[]) {
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char * input_filename = argv[1];
    char * output_filename = NULL;
    char * output_var_name = NULL;

    if (argc <= 3) {
        //No output_var_name_specified
        copy_and_replace(&output_var_name, input_filename, '.', DEFAULT_VAR_EXTENSION);
    } else {
        output_var_name = argv[3];
    }

    if (argc <= 2) {
        copy_and_replace(&output_filename, input_filename, '.', DEFAULT_EXTENSION);
    } else {
        output_filename = argv[2];
    }


    printf("Input file  : %s\n", input_filename);
    printf("Output file : %s\n", output_filename);
    printf("Var name    : %s\n", output_var_name);


    //Opening files
    FILE* in_file  = fopen(input_filename, "rb");
    FILE* out_file = fopen(output_filename,"w+");

    if(in_file == NULL) {
        printf("Couldn't open %s\n", input_filename);
        exit(EXIT_FAILURE);
    }

    if(out_file == NULL) {
        printf("Couldn't open %s\n", output_filename);
        exit(EXIT_FAILURE);
    }
    
    char* define_str =  strdup(output_var_name);
    strcaseup(define_str);

    //Getting file size
    fseek(in_file, 0, SEEK_END);
    long in_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);

    //Writing header to out_file
    
    fprintf(out_file, "//Auto-generated file\n");
    fprintf(out_file, "#ifndef %s\n", define_str);
    fprintf(out_file, "#define %s\n", define_str);
    fprintf(out_file, "\n");
    fprintf(out_file, "static const long %s%s = %ld;\n", output_var_name, LEN_SUFFIX, in_size);
    fprintf(out_file, "static const unsigned char %s[%s%s] = {", output_var_name, output_var_name, LEN_SUFFIX);
    
    //Writing data to out_file
    long bytes_written = 0;
    int bytes_written_line = BYTES_PER_LINE;

    while (1) {
	//Reading byte after byte and appending the corresponding hex code
	//to the output file, inserting a line break every BYTES_PER_LINE bytes
        int c = fgetc(in_file);
        
        if(c == EOF){
            break;
        }

        if(bytes_written_line >= BYTES_PER_LINE) {
            fprintf(out_file, "\n" INDENT);
            bytes_written_line = 0;
        }

        fprintf(out_file, "0x%02x, ", c);

        bytes_written_line++;
        bytes_written++;
    }

    fseek(out_file, -2, SEEK_CUR);

    fprintf(out_file, "\n");
    fprintf(out_file, "};\n");
    fprintf(out_file, "\n");
    fprintf(out_file, "#endif");

    fclose(in_file);
    fclose(out_file);

    printf("Dumped %ld/%ld bytes\n", bytes_written, in_size);

    if(!argc>=3) free(output_var_name);
    if(!argc>=2) free(output_filename);

    return EXIT_SUCCESS;
}
