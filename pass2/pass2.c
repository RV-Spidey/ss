#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

int main()
{
  int startaddress,address,finaladdress,length,oplen,s;
  
  char label[10],opcode[10], operand [10],mnemonic[10],code[10],symbol[10],symbaddr[10],ch;
  
  
  printf("opening files\n");
  FILE *intermediate,*objcode,*symtab,*optab,*output;
  intermediate=fopen("intermediate.txt","r");     
  objcode=fopen("objcode.txt","w");
  output=fopen("output.txt","w");
        if(intermediate==NULL||objcode==NULL||output==NULL)  //if error in opening files then shows error
      printf("error in opening files\n");
     
  //reads a line from intermidiate
  fscanf(intermediate,"%s%s%s",label,opcode,operand);  
  if(strcmp(opcode,"START")==0)
      startaddress=atoi(operand);     
  
//goes to last address for finding length 
  while (strcmp(opcode,"END")!=0)
        fscanf(intermediate,"%d%s%s%s",&address,label,opcode,operand);    
  finaladdress=address;
  length=finaladdress-startaddress;

//close and reopen to start from begining 
  fclose(intermediate);
  intermediate=fopen("intermediate.txt","r");
//read a line and is start the write the header record anf first line in objcode and intermedite
  fscanf(intermediate,"%s%s%s",label,opcode,operand);
  if(strcmp(opcode,"START")==0)
  { 
      fprintf(objcode,"H^%s^00%d^00%d\n",label,startaddress,finaladdress);
      fprintf(output,"%s\t%s\t%s\n",label,opcode,operand);
      fprintf(objcode,"T^00%d^%d",startaddress,length);
  }
  fscanf(intermediate,"%d%s%s%s",&address,label,opcode,operand);
  
 while (strcmp(opcode,"END")!=0) 
  {       
      if(strcmp(opcode,"BYTE")==0)
      {
          fprintf(output,"%d\t%s\t%s\t%s\t",address,label,opcode,operand );
          oplen=strlen(operand)-3;  //if byte in form C'EOF' , so removing the count for C,','
          fprintf(objcode,"^");
          for(int i=2;i<(oplen+2);i++)
          {
              s=toascii(operand[i]); //convert to hex
              fprintf(objcode,"%x",s); //eg E->45,0->4F,F->46 Then EOF to 454F46
              fprintf(output,"%x",s);
              
          }
          fprintf(output,"\n");
      }
     // if word print as it is with 0's for padding
     else if(strcmp(opcode,"WORD")==0)
      {
          fprintf(output,"%d\t%s\t%s\t%s\t00000%s\n",address,label,opcode,operand,operand);
          
          fprintf(objcode,"^00000%s",operand);
      }
      //no machine code for resb,resw
     else if(strcmp(opcode,"RESB")==0 || strcmp(opcode,"RESW")==0)
      {
          fprintf(output,"%d\t%s\t%s\t%s\n",address,label,opcode,operand );
      }
      else
      {
          symtab=fopen("symtab.txt","r");
          optab=fopen("optab.txt","r");
          while(fscanf(optab,"%s%s",mnemonic,code)==2)
          {
              if(strcmp(mnemonic,opcode)==0)
              break;
          }
          
          while(fscanf(symtab,"%s%s",symbol,symbaddr)==2)
          {
              if(strcmp(symbol,operand)==0)
              { 
              break;}
          }
          
          fprintf(output,"%d\t%s\t%s\t%s\t%s%s\n",address,label,opcode,operand,code,symbaddr);
          
          fprintf(objcode,"^%s%s",code,symbaddr);
          
          fclose(symtab);
          fclose(optab);
      }
      fscanf(intermediate,"%d%s%s%s",&address,label,opcode,operand);
      
    } 
    
  
     
  fprintf(objcode,"\nE^00%d",startaddress);
  printf("writing complete\n\n");
  fclose(intermediate);
  fclose(objcode);
  fclose(output);  
  
}
