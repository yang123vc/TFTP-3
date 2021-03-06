#define FILE_NAME_MAX_SIZE 1010

#include "Logger.h"
#include "CommandHandler.h"
#include "Tftpmanager.h"

struct command_line_data
{
  char type[3];
	char host[30];
	char port[5];
	char proto[6];
	char req[8];
	char filename[FILE_NAME_MAX_SIZE];

}arguments;

char* copystring(char* str,char* buf)
{
	int i=1,j=0;
	while(buf[i]!=')')
	{
		
		str[j]=buf[i];
		i++;
		j++;
	}
	
	str[j]='\0';

	return str;
}

int parse(int argc,char** argv)
{
	int i;
	//default port and protocol(udp)
	strcpy(arguments.port,"69");
	strcpy(arguments.proto,"udp");

	if(!strcmp(argv[1],"tftp"))
	{
		if(!strcmp(argv[2],"-c"))
		{
			strcpy(arguments.type,"-c");
			for(i=3;i<argc;i=i+2)
			{
				
				if(!strcmp(argv[i],"host"))
					copystring(arguments.host,argv[i+1]);
				if(!strcmp(argv[i],"port"))
					copystring(arguments.port,argv[i+1]);
				if(!strcmp(argv[i],"-proto"))
					copystring(arguments.proto,argv[i+1]);
				if(!strcmp(argv[i],"-read") || !strcmp(argv[i],"-write"))
				{
					strcpy(arguments.req,argv[i]);
					copystring(arguments.filename,argv[i+1]);
				}

			}
			
		return 1;
		}
		else
			if(!strcmp(argv[2],"-s"))
			{
				strcpy(arguments.type,"-s");
				if(argc==5){
				if(!strcmp(argv[3],"port"))
					copystring(arguments.port,argv[4]);}
				else
					return 1;

			}
			else
				return 0;

	}
	else
	{
		return 0;
	}
}

int main(int argc, char **argv)
{	
	
	std::stringstream msg;
	

	CommandHandler *m_Manager;
	m_Manager=new TftpManager;
	
	/*Parse the arguments and store them*/
	if(!parse(argc,argv))
	{
		m_Manager->Help();
		return 0;
	}
		
	/*Initiate the server and clients*/		
	Error_Barrier(m_Manager->Init(arguments.type,arguments.host,arguments.port,arguments.proto));
	
	if(!strcmp(arguments.type,"-c"))
	{
		
	if(!strcmp(arguments.req,"-read"))
	{ 
		m_Manager->Read(arguments.filename);}

	if(!strcmp(arguments.req,"-write"))
		m_Manager->Write(arguments.filename);
	return 0;
	}
	else
		if(!strcmp(arguments.type,"-s"))
		{
		Error_Barrier(m_Manager->Run());		/*Run the server for handling the clients*/
		return 0;
		}
		else
			cout<<"Command not found!!!press h for help.";


	

	return 0;
}
