#include <iostream>
#include <fstream> // untuk file external
#include <string> //untuk to upper
#include <stdlib.h> // untuk variabel rand
#include <time.h> //header untuk random angka biar ga sama terus
using namespace std;

//kamus LinkList
typedef char infotype;
typedef struct TElmtList *address;
typedef struct TElmtList{
    infotype info;
    address next;
} ElmtList;
typedef struct {
    address first;
}List ;

//Kamus Game
struct kataHint{
	string kata;
	string hint;
};
struct scoreUser{
	string uname;
	int score;
};
char udahPilih [26]; //huruf yang udah dipilih, masuk ke array ini
int soalSudah [30]; //soal yang udah di tebak, urutannya masuk ke array ini
int countSoalUdah = -1; //banyak soal yang udah ditebak
int countUdahPilih = -1; //banyaknya huruf yang udah dipilih per-round nya
int maxRank=21;

int nyawa = 4;
int round = 1;
int score = 0;

#define first(L) (L).first
#define next(p) (p)->next
#define info(P) (P)->info

//Link List sub program
bool IsEmpty(List L);
void CreateEmpty(List *L);
address Allocation(infotype x);
void InsertLast(List *L, infotype x);
void print (List L);

//Game sub program
int hitungData(char tipedata); 
void loadData (kataHint soal[], scoreUser rank[], char tipedata);
void random(string *kata, string *hint, kataHint soal[]);
bool listSoalUdah(int angka);
void Game(List *L, List *P, int x,string hint);
bool cekInput (infotype huruf); //u cek apkh input huruf atau bukan
bool gantiPager(List *L, List *P, infotype huruf);
bool cekList(List L, List P);//
bool cekInputString(List L,string kata);//
string nama(string inisial);
void peringkat(kataHint soal[],scoreUser rank[], int banyakRank);//

int main() {
    List data; //list huruf  ex : A P A
    List pager; //list pager ex : # # #
    
    int banyakSoal=hitungData('s');
    kataHint soal[banyakSoal];
    
    int  banyakRank=(hitungData('r')+1); //misal arraynya 5 kalo +1 jadi 6 nah +1 itu untuk nyimpen data si user yg main barusan
    scoreUser rank[banyakRank]; 
    
    loadData(soal,rank,'s');
    
    string kata,hint;
    while (nyawa != 0 && round!=(banyakSoal+1)){
    	random(&kata,&hint,soal);
    	
    	CreateEmpty(&data);
	    CreateEmpty(&pager);
	    int i;
	    for (i=0; i<kata.length();i++){
	    	kata[i]=toupper(kata[i]);
			InsertLast (&data, kata[i]);
			InsertLast (&pager, '#');
		}
		Game(&data, &pager, kata.length(),hint);
		
		round++;
		countUdahPilih=-1;
	}
	
	if(nyawa==0){
		cout << "MAAF KESEMPATAN ANDA TELAH HABIS";
		cout << "\nJAWABAN YANG BENAR ADALAH ";
		print(data);
		cout << "\nSILAHKAN COBA LAIN KALI SAJA YA";
		cout << "\n- G A M E O V E R -\n\n";
	}else if (round==(banyakSoal+1)){
		cout << "\nSELAMAT ANDA TELAH MENEBAK SEMUA TEKA-TEKI\n\n";
	}
	
	peringkat(soal,rank,banyakRank);
}

void peringkat(kataHint soal[],scoreUser rank[], int banyakRank){
    
	loadData(soal,rank,'r');
	string inisial;
		inisial=nama(inisial);
	int New=0;
	
	if (score >= rank[banyakRank-2].score){ //ketika score > score terakhir di file 
		if (banyakRank==maxRank && score==rank[banyakRank-2].score){ //ketika file sudah 20 data && score user == score terakhir
			cout<<"SAYANG SEKALI ANDA HARUSNYA MENDUDUKI PERINGKAT 20\nNAMUN SUDAH ADA PERINGKAT 20 NIHH :(\nSILAHKAN COBA KEMBALI LAIN WAKTU YA";
		}else{
			New=banyakRank-1;
			
			rank[New].uname=inisial;
			rank[New].score=score;
			
			for(int i=banyakRank-1;i>=0;i--){
				if(rank[New].score>rank[i].score){
					int temp;
					
					swap(rank[New].score,rank[i].score);
					swap(rank[New].uname,rank[i].uname);
					temp=New;
					New=i;
					i=temp;
				}	
			}
			cout<<"SELAMAT ANDA ADALAH PERINGKAT KE-"<<New+1;
			
			ofstream save;
			save.open("rank.txt");
	    	int loop;
			if(banyakRank==maxRank){
				loop=banyakRank-1;
			}else{
				loop=banyakRank;
			} 
			
    		for (int i=0;i<loop;i++){
    			if (i==loop-1){
    				save<<rank[i].uname; save<<"\t"<<rank[i].score;
				}else{
					save<<rank[i].uname; save<<"\t"<<rank[i].score<<"\n";
				}
			}
		}
	}else{
		cout<<"MAAF ANDA BELUM MENDUDUKI PERINGKAT 20 BESAR\nSILAHKAN COBA KEMBALI LAIN WAKTU";
	}
	
	int tampilkan;
	A:
	cout<<"\n\nApakah anda ingin melihat peringkat 20 besar ?\n1. YA\t2.TIDAK\nMasukkan pilihan : ";
	cin>>tampilkan;
	
	switch(tampilkan){
		case 1:
			int loop;
			if(banyakRank==maxRank){
				loop=banyakRank-1;
			}else{
				loop=banyakRank;
			} 
			for (int i=0;i<loop;i++){
    			if (i==banyakRank-1){
    				cout<<rank[i].uname; cout<<"\t"<<rank[i].score;
				}else{
					cout<<rank[i].uname; cout<<"\t"<<rank[i].score<<"\n";
				}
			}cout<<"\nTERIMAKASIH TELAH BERMAIN";
			break;
		case 2: 
			cout<<"\nTERIMAKASIH TELAH BERMAIN";
			break;
		default: 
			cout<<"INPUT SALAH";
			system("pause");
			goto A;
	}
}

string nama(string inisial){
	if (inisial.length()==3){
		return inisial;
	}else{
		cout<<"Masukkan Inisial (3 huruf)\n";
		cin>>inisial;
		system("CLS");
		return nama(inisial);
	}
}

void Game(List *L, List *P, int x,string hint){
	string tebak;
	
	do{
		cout<<"Round : "<<round;
		cout<<"\n\nTebak Kata \t\t: ";
    	print(*P);
	    cout<<"("<<x<<" huruf)\n";
	    cout<<"Hint\t\t\t: "<<hint<<"\n";
	    cout<<"Kesempatan menebak \t: "<<nyawa<<" kali\n";
	    cout<<"Skor saat ini \t\t: "<<score<<"\n";
	    cout<<"Masukkan tebakan anda \t: ";
		cin>>tebak;
		for(int i=0;i<tebak.length();i++){
			tebak[i]=toupper(tebak[i]);
		}
		
		cout<<"\nTekan ENTER untuk lanjutkan...";
		cin.get(); cin.ignore();
		system("CLS");
	
		if(tebak.length()==1){
			if(!cekInput(tebak[0])){
				cout<<"Input harus huruf dan belum pernah dipilih ...\n\n";
			}else{
				gantiPager(L,P,tebak[0]);
				if (!gantiPager(L,P,tebak[0])){
					cout<<"tebakan anda salah ... \n\n";
					nyawa--;
				}
			}
		}else if (tebak.length()==x){
			if(!cekInputString(*L,tebak)){
				cout<<"tebakan anda salah ... \n\n";
				nyawa--;
			}else{
				cout<<"SELAMAT TEBAKAN ANDA BENAR\n";
				cout<<"JAWABANNYA ADALAH ";
				cout<<tebak <<"\n";
				score=score+10;
				break;
			}
		}else if(tebak.length()!=1 && tebak.length()!=x){
			cout<<"tebakan anda salah ... \n\n";
			nyawa--;
		}
	}while (!cekList(*L,*P) && nyawa!=0);
	
	if (cekList(*L,*P)){
		cout<<"SELAMAT TEBAKAN ANDA BENAR\n";
		cout<<"JAWABANNYA ADALAH ";
		print(*L);
	}
	
	cout<<"\n\nTekan ENTER untuk lanjutkan...";
	cin.get(); 
	system("CLS");
}

bool cekList(List L, List P){ //untuk cek apakah semua pagar sudah di ubah jadi huruf
	address temp = first(L);
	address tempV = first(P);
	
	while(temp!=NULL){
		if (info(tempV)!=info(temp)){
			return false;
		}temp=next(temp);
		tempV=next(tempV);
	}return true;
}

bool cekInput (infotype huruf){
    if ((huruf<'A'||huruf>'Z')){
		return false;
	}
	for(int i=0;i<=countUdahPilih;i++){
		if (huruf==udahPilih[i]){
			return false;
		}
	}countUdahPilih++;
	udahPilih[countUdahPilih]=huruf;
	return true;
}

bool cekInputString(List L,string kata){
	address temp=first(L);
	int i=0;
	
	for (int i=0;i<kata.length();i++){
		if (kata[i]==info(temp)){
			temp=next(temp);
		}else{
			return false;
		}
	}return true;
}

bool gantiPager(List *L, List *P, infotype huruf) { //untuk ganti pager jadi huruf temp=tempV
	address temp = first(*L);
	address tempV = first(*P);
	bool bener=false;
	
	while (temp!=NULL){
		if (huruf==info(temp)){
			info(tempV)=info(temp);
			bener=true;
		}tempV=tempV->next;
		temp=temp->next;
	}return bener;
}

int hitungData(char tipedata){	//menghitung data dalam file external
	int i=0;
	int tempInt; //untuk score karena tipe datanya integer
	string temp;
	ifstream load;	
	
	if (tipedata=='s'){
		load.open("soal.txt");
		while (!load.eof()){
			load>>temp; load>>temp;
			i++;
		}
	}else if (tipedata=='r'){
		load.open("rank.txt");
		while (!load.eof()){
			load>>temp; load>>tempInt; 
			i++;
		}
	}
	return i;
}

void loadData (kataHint soal[], scoreUser rank[], char tipedata){ //ngeload semua data file external 
	ifstream load;
	int i=0;
	
	if (tipedata=='s'){
		load.open("soal.txt");
		while (!load.eof()){
			load>>soal[i].kata; load>>soal[i].hint;
			i++;
		}
	}else if (tipedata=='r'){
		load.open("rank.txt");
		while (!load.eof()){
			load>>rank[i].uname; load>>rank[i].score;
			i++;
		}
	}
}

void random(string *kata, string *hint, kataHint soal[]){ //untuk random angka (jangan di ubah)
	int i=0;
	int angka=hitungData('s');
	//generate angka (jangan di ubah ja! udah bener ini wah)	
	A:
	srand(time(NULL));
	i=(rand()%angka);
	
	for(int j=0;j<=countSoalUdah;j++){
		if (i==soalSudah[j]){
			goto A;
		}
	}
	*kata = soal[i].kata;
	*hint = soal[i].hint;
	countSoalUdah++;
	soalSudah[countSoalUdah]=i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//subprogram linklist

bool IsEmpty(List L) {
    return ((L).first == NULL);
}
 
void CreateEmpty(List *L) {
    (*L).first = NULL;
}
 
address Allocation(infotype x) {
    address NewElmt;
    NewElmt = (ElmtList*) malloc (sizeof(ElmtList));
 
    info(NewElmt) = x;
    next(NewElmt) = NULL;
 
    return NewElmt;
}
 
void InsertLast(List *L, infotype x) {
    address NewElmt,temp,pred;
    NewElmt = Allocation(x);

	if (NewElmt != NULL){
		if (IsEmpty(*L)){
			first(*L) = NewElmt;
		}else{
			temp=first(*L);
			pred=NULL;
			while ((temp) != NULL) {
				pred=temp;
				temp = next(temp);
	    	}
	    	next(pred)=NewElmt;
		}
	}
}

void print (List L){
	address temp = first(L);
	
    while (temp != NULL) {
        cout << info(temp) <<" ";
        temp = next(temp);
    }
}
