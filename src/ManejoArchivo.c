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

//enum Consistencia{SH,S,JH};
struct MetaData{
	char* consistencia;
	int particiones;
	int tiempoDeCompactacion;
};
void crearBitMapFS(char path[]);
void crearTables(char path[]);
void crearBloquesDatos(char path[]);
void crearUnaTabla(char[],char nombreDeLaTabla[]);
void crearMetaDataTabla(char[]);
void completarDatosDeMetaData(char[]);
void lecturaDatosDeMetaData(char[]);

int main(void) {
	char path[100];
		//char *path_aux = config_get_string_value(config,"PUNTOMONTAJE");
		strcpy(path,"/home/utnso/Laureano/");
		strcat(path,"LISSANDRA_FS/");
		char nombreArchivo[100];

		if(mkdir(path, 0777) != 0){
			printf("No se pudo crear Carpeta\n");
		}
		//printf("%s",a);
		/*struct MetaData data;//Intento de struct
		data.consistencia="SH";
		data.particiones=4;
		data.tiempoDeCompactacion=600000;*/
		//crearMetaDataFS(path);
		crearBitMapFS(path);
		crearTables(path);
		crearUnaTabla("/home/utnso/Laureano/LISSANDRA_FS/Table/","TablaA");//se podria settear el config.log con un campo DIRTABLAS
		lecturaDatosDeMetaData("/home/utnso/Laureano/LISSANDRA_FS/Table/TablaA/");
		//completarDatosDeMetaData("/home/utnso/Laureano/LISSANDRA_FS/Table/TablaA",data);
		//lecturaDatosDeMetaData("/home/utnso/Laureano/LISSANDRA_FS/Table/TablaA/Metadata.bin");
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
	FILE *f = fopen(path_aux, "w+");
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

void crearUnaTabla(char path[],char nombreDeLaTabla[]){
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
	crearMetaDataTabla(path_aux);
}
void crearMetaDataTabla(char path[]){
	char path_aux[100];
	strcpy(path_aux,path);
	strcat(path_aux,"Metadata.bin");
	//completarDatosDeMetaData(path_aux,data);
	completarDatosDeMetaData(path_aux);
}
void completarDatosDeMetaData(char path[]){


	//Intento de struct(recibi Medata data)
	/*FILE* f;
	f=fopen(path,"w");
	if(f)
	fwrite(&data,sizeof(struct MetaData),1,f);
	fclose(f);*/
	//Modo Prueba(la funcion solo recibe path).Este es una opcion.
	char* buffer;
	int nbytes;
	buffer="CONSISTENCY=SC\nPARTITIONS=4\nCOMPACTION_TIME=60000";
	nbytes=strlen(buffer);
	FILE *f = fopen(path, "w+");
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
		char** guardado;
		strcpy(dirMetaData,path);
		strcat(dirMetaData,"Metadata.bin");
		//Intento de meter struct
		/*FILE* f;
		f=open(path,"rb");
		struct MetaData data;
		fread(&data,sizeof(struct MetaData),1,f);
		printf("CONSISTENCY=%d\nPARTITIONS=%e\nCOMPACTION_TIME=%f",data.consistencia,data.particiones,data.tiempoDeCompactacion);
		fclose(f);*/

		int nbytes;
	    char *contenido;
	    char *delimitador="=\n";
	    FILE* f=fopen(dirMetaData,"rb");

	    if(f){

	    fread (&nbytes, sizeof nbytes, 1, f);
	    contenido = (char *)malloc(nbytes+1);
	    fread (contenido, nbytes, 1, f);
	    contenido[nbytes] = 0;
	    //printf("\n%s\n",contenido);
	    char* token=strtok_r(contenido,delimitador,guardado);
	    if(token!=NULL){
	    	while(token!=NULL){
	    	if(strstr(token,"CONSISTENCY")==NULL&&strstr(token,"PARTITIONS")==NULL&&strstr(token,"COMPACTION_TIME")==NULL)
	    			printf("\n%s\n",token);
	    		token=strtok_r(NULL,delimitador,guardado);
	    	}
	    }
	    }
	    fclose(f);
}
