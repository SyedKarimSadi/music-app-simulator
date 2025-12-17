#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// Function Declarations/ Prototypes

int adminLogin(); 
void adminMenu(); 
void userMenu(); 
void addSongRequest(); 
void approveSongs(); 
void viewSongs();  
void playSong(); 
void deleteSong(); 
void searchSong(); 
void rateSong(); 
void pendingSongs(); 
void viewRatings(); 

int main(){
    int choice;
    while(1){
        printf("\n*====== MUSIC APP SIMULATOR ======*\n");
        printf("\n__________________\n");
        printf("| 1. Admin Login |\n");
        printf("__________________\n");
        printf("| 2. User Menu   |\n");
        printf("__________________\n");
        printf("| 3. Exit        |\n");
        printf("__________________\n");
        printf("Enter choice: ");
       
        scanf("%d",&choice);
        while(getchar()!='\n');

        switch(choice){
            case 1:
                if(adminLogin())
                    adminMenu();
                else
                    printf("Invalid credentials!\n");
                break;
            case 2:
                userMenu();
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid input!\n");
        }
    }
    return 0;
}

// Admin login
int adminLogin(){
    char user[100], pass[100];
    printf("Username: ");
    scanf("%19s",user);
    printf("Password: ");
    scanf("%19s",pass);
    while(getchar()!='\n');
    return strcmp(user,"admin")==0 && strcmp(pass,"1234")==0;
}

// Admin menu
void adminMenu(){
    int ch;
    while(1){
        printf("\n--- ADMIN MENU ---\n");
        printf("1. View All Songs\n");
        printf("2. View Pending Songs\n");
        printf("3. Approve Songs\n");
        printf("4. Delete Song\n");
        printf("5. View Song Ratings\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d",&ch);
        while(getchar()!='\n');

        switch(ch){
            case 1:viewSongs(); break;	
            case 2:pendingSongs(); break;
            case 3:approveSongs(); break;
            case 4:deleteSong(); break;
            case 5:viewRatings(); break;
            case 6:return;
            default:printf("Invalid option!\n");
        }
    }
}

// User menu
void userMenu(){
    int ch;
    while(1){
        printf("\n--- USER MENU ---\n");
        printf("1. View Songs\n");
        printf("2. Search Song\n");
        printf("3. Play Song\n");
        printf("4. Request New Song\n");
        printf("5. Rate Song\n");
        printf("6. Back\n");
        printf("Enter choice: ");
        scanf("%d",&ch);
        while(getchar()!='\n');

        switch(ch){
            case 1: viewSongs(); break;
            case 2: searchSong(); break;
            case 3: playSong(); break;
            case 4: addSongRequest(); break;
            case 5: rateSong(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    }
}

// Add song request
void addSongRequest(){
    FILE*fp= fopen("pending.txt","a");
    if(fp==NULL){
        printf("File error!\n");
        return;
    }

    char song[100];
    printf("Enter song name to request: ");
    scanf(" %[^\n]",song);
    fprintf(fp,"%s\n",song);
    fclose(fp);
    printf(" Song sent for admin approval!\n");
}

/*
==========================================================
 FIXED Bug: pending.txt was deleted even when songs remain
==========================================================
*/

void approveSongs(){
    FILE *p = fopen("pending.txt", "r");
    FILE *s = fopen("songs.txt", "a");

    if (p == NULL) {
        printf("No pending songs!\n");
        return;
    }

    if (s == NULL) {
        printf("Error opening songs file!\n");
        fclose(p);
        return;
    }

    char song[100];
    int choice;
    int found = 0;

    FILE *temp = fopen("temp_pending.txt", "w"); 
    if (temp == NULL) {
        printf("File error!\n");
        fclose(p);
        fclose(s);
        return;
    }

    printf("\n--- APPROVE SONG REQUESTS ---\n");

    while (fgets(song, 50, p)) {
        song[strcspn(song, "\n")] = '\0';
        found = 1;

        printf("\nPending Song: %s\n", song);
        printf("1. Approve\n2. Reject\n3. Skip\nChoice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        if (choice == 1) {
            fprintf(s, "%s\n", song);
            printf(" Approved\n");
        }
        else if (choice == 2) {
            printf(" Rejected\n");
        }
        else {
            fprintf(temp, "%s\n", song); // keep skipped songs
            printf(" Skipped (kept in pending)\n");
        }
    }

    fclose(p);
    fclose(s);
    fclose(temp);

    remove("pending.txt");
    rename("temp_pending.txt", "pending.txt");

    if (!found)
        printf("No pending requests!\n");
}

// View approved songs
void viewSongs(){
    FILE *fp = fopen("songs.txt","r");
    if(fp==NULL){
        printf("No songs found!\n");
        return;
    }

    char song[100];
    printf("\n--- SONG LIST ---\n");
    while(fgets(song,50,fp)) {
        printf("%s",song);
    }
    fclose(fp);
}

// Play song
void playSong(){
    FILE*fp= fopen("songs.txt","r");
    if(fp==NULL){
        printf("No songs available!\n");
        return;
    }

    char song[100], name[100];
    int found= 0;

    printf("Enter song name to play: ");
    scanf(" %[^\n]", name);

    while(fgets(song,50,fp)){
        song[strcspn(song,"\n")]= '\0';
        if(strcmp(song,name)==0){
            printf(" Now Playing: %s  \n",song);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if(!found)
        printf("Song not found!\n");
}

// Search song
void searchSong(){
    FILE*fp= fopen("songs.txt","r");
    if(fp==NULL){
        printf("No songs available!\n");
        return;
    }

    char song[100], name[100];
    int found=0;

    printf("Enter song name to search: ");
    scanf(" %[^\n]", name);

    while(fgets(song,50,fp)) {
        song[strcspn(song,"\n")] = '\0';
        if(strcmp(song,name)==0) {
            printf(" Song found: %s\n",song);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if(!found)
        printf(" Song not found\n");
}

// Delete song
void deleteSong(){
    FILE*fp= fopen("songs.txt","r");
    if(fp==NULL){
        printf("No songs available!\n");
        return;
    }

    FILE*temp= fopen("temp.txt","w");
    if(temp==NULL){
        printf("File error!\n");
        fclose(fp);
        return;
    }

    char song[100], del[100];
    int found= 0;

    printf("Enter song name to delete: ");
    scanf(" %[^\n]", del);

    while(fgets(song,50,fp)){
        song[strcspn(song,"\n")]= '\0';
        if(strcmp(song,del)!=0)
            fprintf(temp,"%s\n",song);
        else
            found=1;
    }

    fclose(fp);
    fclose(temp);

    remove("songs.txt");
    rename("temp.txt","songs.txt");

    if(found){
        printf(" Song deleted successfully!\n");
    }else{
        printf(" Song not found!\n");
    }
}

// View pending songs
void pendingSongs(){
    FILE*fp= fopen("pending.txt","r");
    if(fp==NULL){
        printf("No pending songs!\n");
        return;
    }

    char song[100];
    printf("\n--- PENDING SONGS ---\n");
    while(fgets(song,50,fp)){
        printf("%s",song);
    }
    fclose(fp);
}

// Rate song
void rateSong(){
    FILE*fp= fopen("ratings.txt","a");
    if(fp==NULL){
        printf("File error!\n");
        return;
    }

    char song[100];
    int rate;

    printf("Enter song name: ");
    scanf(" %[^\n]",song);
    printf("Rating (1-5): ");
    scanf("%d",&rate);
    while(getchar()!='\n');

    if(rate<1 || rate>5){
        printf("Invalid rating!\n");
        fclose(fp);
        return;
    }

    fprintf(fp,"%s : %d\n",song,rate);
    fclose(fp);
    printf(" Rating submitted!\n");
}

// View ratings
void viewRatings(){
    FILE*fp= fopen("ratings.txt", "r");
    if(fp==NULL){
        printf("No ratings found!\n");
        return;
    }

    char line[100];
    printf("\n--- SONG RATINGS ---\n");
    while(fgets(line, 100, fp)){
        printf("%s", line);
    }
    fclose(fp);
}


