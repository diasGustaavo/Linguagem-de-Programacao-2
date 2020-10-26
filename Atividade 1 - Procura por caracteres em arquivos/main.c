#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int spawn (char* program, char** arg_list)
{
	pid_t child_pid;
	int stat;

	child_pid = fork();
	if(child_pid != 0){
		wait(&stat);
		return WEXITSTATUS(stat);
	}
	else {
	  	execvp(program, arg_list);
	  	fprintf (stderr, "an error occurred in execvp\n");
	  	abort();
	}
}

int main(){
	char palavra[11];
	printf("digite a palavra a ser procurada: ");
	scanf("%s", palavra);

	// waitexample(); 

	char *argv1[] = {"./countStrings", palavra, "0", NULL};
	int qtdeArq1 = spawn(argv1[0], argv1);

	char *argv2[] = {"./countStrings", palavra, "1", NULL};
	int qtdeArq2 = spawn(argv1[0], argv2);

	char *argv3[] = {"./countStrings", palavra, "2", NULL};
	int qtdeArq3 = spawn(argv1[0], argv3);

	char *argv4[] = {"./countStrings", palavra, "3", NULL};
	int qtdeArq4 = spawn(argv1[0], argv4);

	char *argv5[] = {"./countStrings", palavra, "4", NULL};
	int qtdeArq5 = spawn(argv1[0], argv5);

	printf("Numero de ocorrencias nos arquivos: %d\n", (qtdeArq1 + qtdeArq2 + qtdeArq3 + qtdeArq4 + qtdeArq5));

	return 0;
}