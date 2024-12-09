#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//ハッシュサイズは素数で設定すると、テーブルごとの要素数に偏りが無くなるから良いとされている
#define HASHSIZE_name 11
#define HASHSIZE_country 11
#define error_t "This is test_message\n"

// 当時の課題で使ったデータの中身と構造
struct city{
	char *name;
	float lat;
	float lng;
	char *country;
	int pop;
	struct city *next;
};

// ハッシュテーブル(構造体)を二次元配列で定義
struct city *hashtable[HASHSIZE_name][HASHSIZE_country];

// ハッシュサイズを返す関数
int hash(char *key1){
	int hashval1 = 0;
	while(*key1 != '\0'){
		hashval1 += *key1;
		key1++;
	}
	return hashval1 % HASHSIZE_name;
}

// ハッシュテーブルに追加する関数
void add(char *name,float lat,float lng,char *country,int pop){
	struct city *tmp;
	int hashval1 , hashval2;
	hashval1 = hash(name);
	hashval2 = hash(country);
	tmp = malloc(sizeof(struct city));
	tmp -> name = malloc(sizeof(char)*(strlen(name)+1));
	strcpy(tmp -> name , name);
	tmp -> lat = lat;
	tmp -> lng = lng;
	tmp -> country = malloc(sizeof(char)*(strlen(country)+1));
	strcpy(tmp -> country , country);
	tmp -> pop = pop;
	tmp -> next = hashtable[hashval1][hashval2];
	hashtable[hashval1][hashval2] = tmp;
}

// ハッシュテーブルから削除する関数
void dist(){
	printf( "--HASHSIZE:[%d][%d]\n" , HASHSIZE_name,HASHSIZE_country );

	int hashval1 = 0;
	int hashval2 = 0;
	int counter = 0;
	struct city *ptr;

	while(hashval1 < HASHSIZE_name){
		while(hashval2 < HASHSIZE_country){
			ptr = hashtable[hashval1][hashval2];
			while ( ptr != NULL){
				counter++;
				ptr = ptr -> next;
			}
			printf("TABLE[%d][%d]:%d\n", hashval1, hashval2, counter);
			hashval2++;
			counter = 0;
		}
		hashval1++;
		hashval2 = 0;
	}
}

// 特定の都市データを表示する関数
void print_city(char *name){
	struct city *ptr;
	int hashval1 , hashval2;

	hashval1 = hash(name);
	hashval2 = 0;

	while(hashval2 < HASHSIZE_country){
		ptr = hashtable[hashval1][hashval2];
		while(ptr != NULL){
			if( strcmp( ptr -> name , name ) == 0){
				printf("City Name:%s lat:%f lng:%f [%s] Population:%d\n" , ptr->name , ptr->lat , ptr->lng , ptr->country , ptr->pop);
				return;
			}
			ptr = ptr -> next;
		}
		hashval2++;
	}
	printf("None.\n");
}

// 各都市のソートをする関数
struct city *soating(struct city *city_list){
	struct city *new_list, *tmp;
	new_list = malloc(sizeof(struct city));
	tmp = malloc(sizeof(struct city));
	new_list = NULL;
	while(city_list != NULL){
		struct city *max;
		max = malloc(sizeof(struct city));
		/*
        何でコメントアウトしてるか忘れたけど、エラーの原因分析のためにここを切り分けたんやと思う
		max : city_list 's max
		tmp : city_list free-pointer

		max > tmp : tmp = city_list->next
		max < tmp : max <-> tmp
		tmp == NULL  ~=  city_list == NULL
		new_list = max

		****city_list = city_list - max
		*/
	}
	return new_list;
}

// 該当する都市を繰り返し表示する関数
void print_cities(char *country){
	struct city *ptr, *city_list;
	int hashval1 , hashval2;
	city_list = malloc(sizeof(struct city));
	city_list = NULL;

	hashval1 = 0;
	hashval2 = hash(country);

	printf("[%s cities]\n",country);
	while(hashval1 < HASHSIZE_name){
		ptr = hashtable[hashval1][hashval2];
		while(ptr != NULL){
			if( strcmp( ptr -> country , country ) == 0){
				struct city *tmp;
				tmp = malloc(sizeof(struct city));
				tmp -> name = malloc(sizeof(char)*(strlen(ptr->name)+1));
				strcpy(tmp -> name , ptr->name);
				tmp -> lat = ptr->lat;
				tmp -> lng = ptr->lng;
				tmp -> country = malloc(sizeof(char)*(strlen(country)+1));
				strcpy(tmp -> country , country);
				tmp -> pop = ptr->pop;
				tmp->next = city_list;
				city_list = tmp;
			}
			ptr = ptr -> next;
		}
		hashval1++;
	}
}

// メイン関数
int main(){
	char name[1024];
	char country[1024];
	float lat, lng;
	int pop, i, j, f;
	char *filename = "worldcities.txt";
	FILE *fp;
	char buffer[1024];

	for(i=0; i<HASHSIZE_name; i++){
		for(j=0;j<HASHSIZE_country; j++){
			hashtable[i][j]=NULL;
		}
	}
	fp = fopen(filename,"r");

	while((f = fscanf(fp,"%s %f %f %s %d",name,&lat,&lng,country,&pop))!=EOF){
		add(name, lat, lng, country, pop);
	}
	fclose(fp);
	//dist();

	printf("City name? ");
	scanf("%s", buffer);
	print_city(buffer);

	printf("Country name? ");
	scanf("%s", buffer);
	print_cities(buffer);

	return 0;
}
