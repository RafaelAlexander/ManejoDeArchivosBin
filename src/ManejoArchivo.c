/*
 ============================================================================
 Name        : ManejoArchivo.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


void crearBitMapFS(char path[]);
void crearTables(char path[]);
void crearBloquesDatos(char path[]);
void crearUnaTabla(char path[],char nombreDeLaTabla[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones);
void crearMetaDataTabla(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones);
void completarDatosDeMetaData(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones);
void lecturaDatosDeMetaData(char path[]);
enum  TDeCons{SH=0,SHC=1,EC=2};
int main(void) {
	char path[100];
		//char *path_aux = config_get_string_value(config,"PUNTOMONTAJE");
		strcpy(path,"/home/utnso/Laureano/");
		strcat(path,"LISSANDRA_FS/");
		uint8_t codDeConsistencia = SH;
		u_int tiempoDeCompactacion=60000;
		u_int particiones=4;
		char* nombreDeLaTabla="TablaA";
		if(mkdir(path, 0777) != 0){
			printf("No se pudo crear Carpeta\n");
		}
		crearBitMapFS(path);
		crearTables(path);
		crearUnaTabla("/home/utnso/Laureano/LISSANDRA_FS/Table/",nombreDeLaTabla,codDeConsistencia,tiempoDeCompactacion,particiones);//se podria settear el config.log con un campo DIRTABLAS
		lecturaDatosDeMetaData("/home/utnso/Laureano/LISSANDRA_FS/Table/TablaA/");
		crearBloquesDatos(path);
	return EXIT_SUCCESS;
}

void crearBitMapFS(char path[]){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux, "Metadata/");
	if(mkdir(path_aux, 0777) != 0){
			printf("No se pudo crear la carpeta /Metadata (Puede que ya este creada)\n");
	}
	strcat(path_aux,"Bitmap.bin");
	FILE *f = fopen(path_aux, "wb+");
	fclose(f);
	//<commons/bitarray.h>
}
void crearTables(char path[]){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux, "Table/");
	if(mkdir(path_aux, 0777) != 0){
		printf("No se pudo crear la carpeta /Tables.\n");
	}
}

void crearBloquesDatos(char path[]){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux, "Bloques/");
	if(mkdir(path_aux, 0777) != 0){
		printf("No se pudo crear /Bloques\n");
	}
}

void crearUnaTabla(char path[],char nombreDeLaTabla[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones){
	char path_aux[100];
	char nombre_aux[50];
	strcpy(nombre_aux,nombreDeLaTabla);
	strcpy(path_aux,path);
	strcat(nombre_aux,"/");
	strcat(path_aux,nombre_aux);
	if(mkdir(path_aux, 0777) != 0){
			printf("No se pudo crear Tabla");
	}
	//crearMetaDataTabla(path_aux,data);
	crearMetaDataTabla(path_aux,tipoDeConsistencia,tiempoDeCompactacion,particiones);
}
void crearMetaDataTabla(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux,"Metadata.bin");
	//completarDatosDeMetaData(path_aux,data);
	completarDatosDeMetaData(path_aux,tipoDeConsistencia,tiempoDeCompactacion,particiones);
}
void completarDatosDeMetaData(char path[],uint8_t tipoDeConsistencia,u_int tiempoDeCompactacion,u_int particiones){
	char buffer[100];
	int nbytes;
	//buffer="CONSISTENCY=SC\nPARTITIONS=4\nCOMPACTION_TIME=60000";
	printf("\n%d\n",tipoDeConsistencia);
	printf("\n%d\n",tiempoDeCompactacion);
	printf("\n%d\n",particiones);
	//sprintf(buffer,"PARTITIONS=%d\nCOMPACTION_TIME=%d\n",particiones,tiempoDeCompactacion);
	switch(tipoDeConsistencia){
	case SH:
		//Hice este sprintf para probar que no habia error
		printf("Entre");
		sprintf(buffer,"CONSISTENCY=SC\nPARTITIONS=%d\nCOMPACTION_TIME=%d",particiones,tiempoDeCompactacion);
		break;
	case SHC:
		sprintf(buffer,"CONSISTENCY=SHC\nPARTITIONS=%d\nCOMPACTION_TIME=%d",particiones,tiempoDeCompactacion);
		break;
	case EC:
		sprintf(buffer,"CONSISTENCY=SC\nPARTITIONS=%d\nCOMPACTION_TIME=%d\n",particiones,tiempoDeCompactacion);
		break;
	default:
		printf("\nRecibi tipo de consistencia invalida");
	}
	nbytes=strlen(buffer);
	FILE *f = fopen(path, "wb+");
	if(f){
	//Mando tamaño de string
	fwrite(&nbytes,sizeof nbytes,1,f);
	//Mando String
	fwrite(buffer,nbytes,1,f);
	}
	fclose(f);
}
void lecturaDatosDeMetaData(char path[]){
		char dirMetaData[100];
		//char** guardado;
		strcpy(dirMetaData,path);
		strcat(dirMetaData,"Metadata.bin");
		int nbytes;
	    char *contenido;
	    char *delimitador="=\n";
	    FILE* f=fopen(dirMetaData,"rb");

	    if(f){

	    fread (&nbytes, sizeof nbytes, 1, f);
	    contenido = (char *)malloc(nbytes+1);
	    fread (contenido, nbytes, 1, f);
	    contenido[nbytes] = 0;
	    printf(contenido);
	   // char* token=strtok_r(contenido,delimitador,guardado);
	    char* token=strtok(contenido,delimitador);
	    if(token!=NULL){
	    	while(token!=NULL){
	    	if(strstr(token,"CONSISTENCY")==NULL&&strstr(token,"PARTITIONS")==NULL&&strstr(token,"COMPACTION_TIME")==NULL)
	    			printf("\n%s\n",token);
	    		//token=strtok_r(NULL,delimitador,guardado);
	    		token=strtok(NULL,delimitador);
	    	}
	    }
	    }
	    fclose(f);
}