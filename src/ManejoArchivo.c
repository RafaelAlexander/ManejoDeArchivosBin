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

/*typedef struct{
	char* consistencia;
	int particiones;
	int tiempoDeCompactacion;
}MetaData;*/

int main(void) {
	char path[100];
		//char *path_aux = config_get_string_value(config,"PUNTOMONTAJE");
		strcpy(path,"/home/utnso/Laureano/LISSANDRA_FS/");
		char nombreArchivo[100];

		if(mkdir(path, 0777) != 0){
			printf("No se pudo crear Carpeta\n");
		}
		/*MetaData data;
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

	/*
	//Intento de struct(recibi Medata data)
	FILE* f;
	f=fopen(path,"ab");
	fwrite(&data,sizeof(MetaData),1,f);
	fclose(f);*/
	//Modo Prueba(la funcion solo recibe path).Este es una opcion.
	char* buffer;
	int nbytes;
	buffer="CONSISTENCY=SC\nPARTITIONS=4\nCOMPACTION_TIME=60000";
	nbytes=strlen(buffer);

	FILE *f = fopen(path, "wb+");
	//Mando tamaño de string
	fwrite(&nbytes,sizeof nbytes,1,f);
	//Mando String
	fwrite(buffer,nbytes,1,f);
	fclose(f);
}
void lecturaDatosDeMetaData(char path[]){

		char dirMetaData[100];
		strcpy(dirMetaData,path);
		strcat(dirMetaData,"Metadata.bin");
		char** guardadoDeToken;
		/*
		//Intento de meter struct
		FILE* f;
		f=open(path,"rb");
		MetaData data;
		fread(&data,sizeof(MetaData),1,f);
		printf("CONSISTENCY=%d\nPARTITIONS=%e\nCOMPACTION_TIME=%f",data.consistencia,data.particiones,data.tiempoDeCompactacion);
		fclose(f);*/

		int nbytes;
	    char *contenido;
	    FILE* f=fopen(dirMetaData,"rb");
	    fread (&nbytes, sizeof nbytes, 1, f);
	    contenido = (char *)malloc(nbytes+1);
	    fread (contenido, nbytes, 1, f);
	    contenido[nbytes] = 0;
	    printf("\n%s\n",contenido);
}
