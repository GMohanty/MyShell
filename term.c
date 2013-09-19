#include<stdio.h>
#include<string.h>
#include<signal.h>
#include<sys/utsname.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/wait.h>
/********************************************************************************
  STRUCTURE TO STORE MY PROCESS DETAILS !!!
 ********************************************************************************/
typedef struct node{
    char proname[100];
    int backflag;
    int pro_id;
}node;
/********************************************************************************
  ENDS STRUCTURE HERE !
 *********************************************************************************/




/********** GLOBAL DECLARATIONS SECTION******************/

typedef void(*sighandler_t)(int);
char *getcwd(char *buf, size_t size);
char *x;
char prime[1024];
int back=0;
int pi;
int fore;
int ptx=0;
node pot[100];
char *usrn;
int rein,reout,repipe;
char Inpre[100000];
char Outre[100000]; 
/******************************************************/




/********Handle Signals**********************************/

void handle_sig()
{
    char host[100],cwd[1024];
    gethostname(host,sizeof(host));
    printf("\n<%s@%s:%s$> ",usrn,host,x);
    fflush(stdout);        
} 
void c_sigh()
{
    int c_id,i;char host[100];
    int fl=0;
    c_id=waitpid(WAIT_ANY,NULL,WNOHANG);
    for(i=0;i<ptx;i++)
    {
        if(pot[i].pro_id==c_id)
        {  pot[i].backflag=-1;
            fl=1;
            printf("\nProcess %s (Process ID:%d) has been terminated.\n",pot[i].proname,c_id);
            break;
        }
    }
    if(fl==1)
    {
        gethostname(host,sizeof(host));
        printf("<%s@%s:%s$> ",usrn,host,x);
        fflush(stdout);        
    }

}
void hnd()
{
    if(fore>0)
    {
        kill(fore,SIGTSTP);
    }

}
/*******************************************************/



/********************************************************************************
  Function For Parsing the command from prompt :
 **********************************************************************************/

int parse(char *inputString, char *cmdArgv[])
{  // printf("Parsing Begins !\n");
    int cmdArgc = 0;
    char *srcPtr = inputString;
    int flag=0;
    char *tmp;
    while(*srcPtr != '\0')
    {
        *cmdArgv = srcPtr;
        cmdArgc++;
        while(*srcPtr != ' ' && *srcPtr != '\t' && *srcPtr != '\0' && *srcPtr != '\n' && *srcPtr!='>' && *srcPtr!='<')
        {

            srcPtr++;
        }
        while((*srcPtr == ' ' || *srcPtr == '\t' || *srcPtr == '\n') )
        {
            *srcPtr = '\0';
            srcPtr++;
        }
        if(*srcPtr=='<'||*srcPtr=='>')
        {     
            if(*srcPtr=='<')
                flag=1;
            else
                flag=2;
            *srcPtr='\0';   
            srcPtr++;
            tmp=srcPtr;
            srcPtr--;     
        }


        cmdArgv++;
    }
    *cmdArgv = '\0';
    //printf("Parsing Ends %d\n",cmdArgc);
    //   printf("YES\n");
    if(flag>0)
    {
        int i=0;
        if(flag==1)

        {rein=1;
            //printf("tmp is <");
            while(*tmp==' ' || *tmp=='\t')
            {    tmp++; }
            while(*tmp!=' ' && *tmp!='>' && *tmp!='\t' && *tmp!='\0')
            {
                Inpre[i]=*tmp;
                i++;
                tmp++;
            }
            Inpre[i]='\0';
            while(*tmp==' ')
            {tmp++;
            }
            //printf("I%s\n",Inpre);
        }
        if(flag==2 || flag==1)
        {
            if(flag==2)
            { 
                reout=1;
                while(*tmp==' '|| *tmp=='\t')
                {
                    tmp++;
                }
                i=0;
                while(*tmp!=' ' && *tmp!='\t' && *tmp!='\0')
                { Outre[i]=*tmp;
                    i++;
                    tmp++;
                }
                Outre[i]='\0';

            }
            else if(*tmp=='>')
            {   tmp++;
                reout=1; 
                while(*tmp==' '|| *tmp=='\t')
                {
                    tmp++;
                }
                i=0;
                while(*tmp!=' ' && *tmp!='\t' && *tmp!='\0')
                { Outre[i]=*tmp;
                    i++;
                    tmp++;
                }
                Outre[i]='\0';
            }
        }
    }
    return cmdArgc;


}
/**********************************************************************************
  ENDS PARSING HERE !!
 **********************************************************************************/


/**********************************************************************************************
  Function for commands : cd(Change Directory) | quit | non-pipe commands. |  user-def commands
 **********************************************************************************************/
void func(char **arr,int len)
{  
    int pid;
    if(strcmp(arr[0],"quit")==0)
        return;
    else if(strcmp(arr[0],"cd")==0)
    {
        if(len==1 || strcmp(arr[1],"~")==0)
        {
            char def[10];
            chdir(prime);
            x=strcpy(def,"~");
            return;

        }
        char pr[1024];
        //printf("%s\n",arr[1]);
        int f;
        f=chdir(arr[1]);
        if(f==-1)
        {
            printf("No such directory exists!\n");
            return;
        }
        getcwd(pr,sizeof(pr));
        //printf("%s is the path of current working directory\n",pr);
        if(strcmp(prime,pr)>0)
        {
            x=strrchr(pr,'/');
            //printf("%s\n",x);
            return;
        }
        else if(strcmp(prime,pr)<0)
        {
            int diff=strlen(pr)-strlen(prime);
            //printf("1");
            int start=strlen(prime);
            x=strndup(pr+start,diff);
            char mn[1024];
            strcpy(mn,"~");
            strcat(mn,x);
            strcpy(x,mn);
            //printf("%s\n",x);
            return;

        }
        else if(strcmp(prime,pr)==0)
        {
            char mn[10];
            x=strcpy(mn,"~");
            return;   
        }



    }
    else if(strcmp(arr[0],"pinfo")==0)
    {   
        char buf[30];
        char bufx[100];
        if(len==1)
        {    sprintf(buf,"/proc/%d/status",pi);
            sprintf(bufx,"/proc/%d/exe",pi);   
        }
        else if(len==2)       
        { 
            sprintf(buf,"/proc/%s/status",arr[1]);
            sprintf(bufx,"/proc/%s/exe",arr[1]);   
        }
        else if(len>2)
            printf("More than required number of arguments : Only 1 extra argument allowed with pinfo\n");
        FILE *file=fopen(buf,"r");
        if(file!=0)
        {int y;
            int ctr=1;
            while(ctr<13)
            {   
                y=fgetc(file);
                if(ctr==1 || ctr==2 ||ctr ==4 || ctr==12)
                    printf("%c",y);
                if(y=='\n')
                    ctr++;

            }
            char buf2[1024];

            /* The manpage says it won't null terminate.  Let's zero the buffer. */
            memset(buf2, 0, sizeof(buf2));

            /* Note we use sizeof(buf)-1 since we may need an extra char for NUL. */
            if (readlink(bufx, buf2, sizeof(buf2)-1))
            {
                /* There was an error...  Perhaps the path does not exist
                 * or the buffer is not big enough.  errno has the details. */
                //perror("readlink");
            }
            printf("Executable Path :%s\n",buf2);
        }   
    }

    else if(strcmp(arr[0],"jobs")==0)
    {
        int no;
        int cn=0;
        for(no=0;no<ptx;no++)
        {
            if(pot[no].backflag==1)
            {cn=1;
                printf("[%d] %s [%d]\n",no,pot[no].proname,pot[no].pro_id); 
            }   
        }
        if (cn==0)
            printf("No Present Jobs at the moment.\n");
    }

    else if(strcmp(arr[0],"kjob")==0)
    {
        if(len>3)
        {
            printf("More number of arguments in kjob than allowed\n");
            return;
        }
        if(len<3)
        {   printf("Less number of arguments in kjob than expected\n");    
            return;
        }
        long int Jn=strtol(arr[1],NULL,10);
        long int Sn=strtol(arr[2],NULL,10);
        if(pot[Jn].backflag==1)
            kill(pot[Jn].pro_id,Sn);
        else
            printf("Either Foreground Process or no such process exists anymore, Hence kjobs wont work\n");    
    }

    else if(strcmp(arr[0],"fg")==0)
    {
        long int Jn=strtol(arr[1],NULL,10);
        if(pot[Jn].backflag!=1) // Implys that not a back job.
            printf("Not  an existing background process");
        else
        {
            pot[Jn].backflag=0;
            int status;
            waitpid(pot[Jn].pro_id,&status,0);

        }

    }


    else if(strcmp(arr[0],"overkill")==0)
    {   int no;
        for(no=0;no<ptx;no++)
        {
            if(pot[no].backflag==1)
            { pot[no].backflag=-1;
                kill(pot[no].pro_id,SIGKILL);
            }
        }
    }
    else{
        int lo;
        pid=fork();
        if(pid==0)
        {   if(rein==1)
            {
                FILE *fi=fopen(Inpre,"r");
                dup2(fileno(fi),0);
                fclose(fi);
            }
            if( reout==1)
            {
                FILE *fo=fopen(Outre,"w");
                dup2(fileno(fo),1);
                fclose(fo);
            }
            if( execvp(arr[0],arr)==-1)
            { printf("No such command exists or arguments specified are wrong(No permissions or files do not exist)\n");
                _exit(1); 
            }

            //_exit(1);
        }
        else if(pid>0)
        {

            if(back==0)
            {
                /************** ALLOCATE DETAILS OF FOREGROUND PROCESS IN ARRAY ************/
                pot[ptx].pro_id=pid;       
                pot[ptx].backflag=back;
                for(lo=0;lo<len;lo++)
                { //printf("%s\n",arr[lo]);
                    strcat(pot[ptx].proname,arr[lo]);
                    if(len!=1 && lo!=len-1)
                        strcat(pot[ptx].proname," ");
                }
                ptx++;
                fore=pid;
                int status;
                waitpid(pid,&status,WUNTRACED);
                if(WIFSTOPPED(status))
                {
                    pot[ptx-1].backflag=1;

                }

            }
            else{
                /************ALLOCATE DETAILS OF BACKGROUND PROCESS IN ARRAY ************/ 
                pot[ptx].pro_id=pid;       
                pot[ptx].backflag=back;
                for(lo=0;lo<len;lo++)
                { //printf("%s\n",arr[lo]);
                    strcat(pot[ptx].proname,arr[lo]);
                    if(len!=1 && lo!=len-1)
                        strcat(pot[ptx].proname," ");
                }
                ptx++;


            }

        }


    }


}
/**************************************************************************
  ENDS HERE 
 **************************************************************************/


/*************************************************************************
  PIPE Handler
 **************************************************************************/
void pipe_handle(char inp[])
{   char *IndComm[1000]; // Individual commands seperated by pipes
    int CommCount=0;     // Ind. Command Count.
    int pid, i,j;
    int newpipe[2];
    int oldpipe[2];
    int status;

    /**********
      Handle Stdin and Stdout files ( make copies for end-restore
     ***********/
    int stdicpy=dup(STDIN_FILENO);
    int stdocpy=dup(STDOUT_FILENO);
    /***********/


    //printf("(%s)\n",inp);
    char *tok=strtok(inp,"|");
    while(tok!=NULL)
    {    
        tok[strlen(tok)]='\0';
        IndComm[CommCount]=tok;
        CommCount++;
        tok=strtok(NULL,"|");
    }
    IndComm[CommCount]=tok;
    int cmct[100];
    for(i=0;i<CommCount;i++)
    {   rein=0;
        reout=0;
        char *subArgv[100];
        cmct[i]=parse(IndComm[i],subArgv);
        
        /****Debug Prime *******/
        //for(j=0;j<cmct[i];j++)
          //  printf("subArgv[%d][%d]:(%s) [%d,%d][(%s) (%s)]\n ",i,j,subArgv[j],rein,reout,Inpre,Outre);
    if(i==0 && rein==1)
        {FILE *Fp=fopen(Inpre,"r");
        dup2(fileno(Fp),0);
        fclose(Fp);
        }
    if(i==CommCount-1 && reout==1)
        {FILE *Fp=fopen(Outre,"w");
        dup2(fileno(Fp),1);
        fclose(Fp);
        }
      


    
    if(i<CommCount-1)
        pipe(newpipe);
    pid=fork();
    if(i!=0 && i!=CommCount-1)
    {
    dup2(oldpipe[0],0);
    close(oldpipe[1]);
    close(oldpipe[0]);
    }
    else if(i==CommCount-1)
    {
        dup2(oldpipe[0],0);
        close(oldpipe[1]);
        close(oldpipe[0]);
    }
    if(pid==0)
    {
        if(i==0) // First command.. Only possibility for < redirection.
        {
        dup2(newpipe[1],1);    // set stdout as newpipe[1].. all data will put written into this pipe from previous command.
        close(newpipe[0]);
        close(newpipe[1]);
         }
        if(i!=0 && i!=CommCount-1) // Any command other than first and Last Command.
        {
        dup2(newpipe[1],1);    // same as previous.
        close(newpipe[0]);
        close(newpipe[1]);
         }
        if(execvp(subArgv[0],subArgv)<0) // Execution of command.
        { printf("No such command or pipe syntax exists.");
        _exit(1);
        }


    }
    else
    {
        waitpid(pid,&status,0); 
        if(i<CommCount-1)       // Pipe Transition Handler.
        {
            oldpipe[0]=newpipe[0];
            oldpipe[1]=newpipe[1];


        }

    }


    }
/* Stdin and Stdout End restoration */
dup2(stdicpy,0);
close(stdicpy);
dup2(stdocpy,1);
close(stdocpy);
    /* Closing all pipes*/
close(oldpipe[0]);
close(newpipe[0]);
close(oldpipe[1]);
close(newpipe[1]);



}
/*************************************************************************
  Pipe Handler ends here
 **************************************************************************/
int main()
{      
    pi=getpid();       // Take Main process PID.
    usrn=getenv("USER"); // Take username.
    signal(SIGINT,SIG_IGN); 
    signal(SIGTSTP,hnd);   
    printf("\033[2J"); /* Initial Clear Screen */
    printf("\033[1;1H");
    //  signal(SIGINT,handle_sig);
    signal(SIGCHLD,SIG_IGN);
    signal(SIGCHLD,c_sigh);
    char host[100];
    //printf("%d\n",pi);

    gethostname(host,sizeof(host));
    getcwd(prime,sizeof(prime));
    char def[10];
    x=strcpy(def,"~");
    printf("<%s@%s:%s$> ",usrn,host,x);
    char c;
    while(1)
    {   

        int i;
        rein=0;
        reout=0;
        repipe=0;
        char comm[1024];
        int lencomm=0;
        c=getchar();

        if(c=='\n')
        {
            printf("<%s@%s:%s$> ",usrn,host,x);
            fflush(stdout);
            continue;
        }
        int pipfl=0;
        while(c!='\n')  // comm extracts entire
        {   
            if(c=='|')
               { repipe++;pipfl=0;
               }
            comm[lencomm]=c;
            lencomm++;
            c=getchar();
            if(repipe>0 && pipfl==0)
            {pipfl=1;
            while(c==' ')
                c=getchar();

            }
        }
        comm[lencomm]='\0';

        if(repipe>0)
        {
            pipe_handle(comm);
            for(i=0;i<1022;i++) // Set comm empty.
            {   Outre[i]='\0';
                Inpre[i]='\0';
                comm[i]='\0';
            }
            printf("<%s@%s:%s$> ",usrn,host,x); 
            fflush(stdout);
            continue;
        }

        if(comm[lencomm-1]=='&')
        {        comm[lencomm-1]='\0';
            back=1;}
        else
            back=0;

        char *cmdArgv[100];
        char *data;    
        int countargs=0;
        int cmdArgc;

        cmdArgc=parse(comm,cmdArgv); // Parse
        int dirin=0;
        int dirout=0;
        int dir=0;
     
        func(cmdArgv,cmdArgc);

        if(strcmp(cmdArgv[0],"quit")==0) // Execution of Quit
        { 
            return 0;
        }
        else
        {

            printf("<%s@%s:%s$> ",usrn,host,x); 
            fflush(stdout);
        }

        for(i=0;i<1022;i++) // Set comm empty.
        {
            {   Outre[i]='\0';
                Inpre[i]='\0';
                comm[i]='\0';
            }
        }

}
        return 0;
}

