#include<string.h>
#define OFFSET_FIRST 0
#define OFFSET_SECOND 1
#define OFFSET_THIRD 2
#define OFFSET_FOURTH 3
#define TAMANIO_MAT_BYTES 3*width*height
#define CANT_BYTES_MAT_MESSAGE 4*size

//Asumiendo que size es la cantidad de chars del mensaje.
//Tambien voy a asumir cant_bytes_mat_message < tamanio_mat_bytes
static inline unsigned char modificarSegunOp(unsigned char code, unsigned char inputOp);

static inline unsigned char modificarSegunOp(unsigned char op, unsigned char code){
	unsigned char res;
	switch(op){
		case 0://00000000
			res=code;
			break;
		case 1://00000001
			//res=(code == 3) ? 0: (code+1)%4;
			res=(code+1)%4;
			break;
		case 2://00000010
			res = (code>0) ? ((code-1)%4) : 3;
			break;
		case 3://00000011
			res=~(code);			
			res = res & 3;//00000011 = 3 ;limpio los unos que quedaron en la parte alta despues del neg
			break;
	}
	return res;
}

void decode_c(unsigned char* src,
              unsigned char* code,
			  int size,
              int width,
              int height)
{
	int j=0;
	//inicializo code con nulls
	memset(code, '\0', size);
	//proceso el mensaje levantando de a 4 bytes
	for(int index=0;index<CANT_BYTES_MAT_MESSAGE;index+=4){
		//leo los 4 bytes que forman un char

		unsigned char fstByte = src[index+OFFSET_FIRST];
		unsigned char sndByte = src[index+OFFSET_SECOND];
		unsigned char trdByte = src[index+OFFSET_THIRD];
		unsigned char frtByte = src[index+OFFSET_FOURTH];

		//obtengo el code
		unsigned char codeFst = (fstByte & 3);//00000011 = 3
		unsigned char codeSnd = (sndByte & 3);//00000011 = 3
		unsigned char codeTrd = (trdByte & 3);//00000011 = 3
		unsigned char codeFrt = (frtByte & 3);//00000011 = 3		

		//obtengo los op
		unsigned char opFst = (fstByte & 12) >> 2;//00001100 = 12
		unsigned char opSnd = (sndByte & 12) >> 2;//00001100 = 12
		unsigned char opTrd = (trdByte & 12) >> 2;//00001100 = 12
		unsigned char opFrt = (frtByte & 12) >> 2;//00001100 = 12

		//proceso los chars salientes segun los codes.
		unsigned char salienteFst = modificarSegunOp(opFst, codeFst);
		unsigned char salienteSnd = modificarSegunOp(opSnd, codeSnd);
		unsigned char salienteTrd = modificarSegunOp(opTrd, codeTrd);
		unsigned char salienteFrt = modificarSegunOp(opFrt, codeFrt);

		//armo el char saliente
		unsigned char final = (salienteFrt << 6) | (salienteTrd << 4) | (salienteSnd << 2) | (salienteFst);
		code[j] = final;
		j++;
	}
}
