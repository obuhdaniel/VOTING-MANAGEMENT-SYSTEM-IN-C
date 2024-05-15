#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


#define MAX_CANDIDATES 100



struct User
{
    char username[50];
    char password[50];
    char role[20];
    int voted;
};

struct Candidate
{

    char name[50];
    int votes;
};

void registerUser();
void loginUserInterface();
void displayCandidates();
void vote();
void showResults();



int main(){
    int choice;

    printf("welcome to Online voting System");

    do {
        printf("\nMain Menu\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Display Candidates\n");
        printf("4. Vote\n");
        printf("5. Show Results\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUserInterface();
                break;
            case 3:
                displayCandidates();
                break;
            case 4:
                vote();
                break;
            case 5:
                showResults();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);

    return 0;
    
}

bool isUsernameUnique(const char* username){
    FILE* userFile;
    char storedUsername[50];

    userFile = fopen("users.txt", "r");
    if(userFile == NULL){
        printf("Error Opening file. \n");
        return false;
    }
    while (fscanf(userFile, "%49[^,],%*s\n", storedUsername) ==1)
    {
        if (strcmp(username, storedUsername) == 0)
        {
            fclose(userFile);
            return false;
        }
        
    }
    fclose(userFile);
    return true;
}

void registerUser(){
    FILE *userFile;
    struct User newUser;


    userFile = fopen("users.txt", "a");
    if (userFile == NULL)
    {
        printf("Error retrieving Datatbase.\n");
        return;

    }
    printf("\nEnter username: ");
    scanf("%s", newUser.username);

    if (!isUsernameUnique(newUser.username)){
        printf("username already exists. please choose a different username");
        fclose(userFile);
        registerUser();
        return;

    }
    {
        /* code */
    }
    
    printf("Enter password: ");
    scanf("%s", newUser.password);

    fprintf(userFile, "%s,%s,0\n", newUser.username, newUser.password);

    fclose(userFile);
    printf("User successfully registered");
    
}


bool loginUser(const char* username, const char* password){
    FILE *userFile;
    char storedUsername[50];
    char storedPassword[50];
    bool found = false;


    userFile = fopen("users.txt", "r");
    if (userFile == NULL)
    {
        printf("Error opening File.\n");
        return false;
    }
    while (fscanf(userFile, "%49[^,],%49[^,],%*d\n", storedUsername, storedPassword) == 2)
    {
        if(strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0){
            found = true;
            break;
        }
    }
    fclose(userFile);

    if (found) {
        printf("Login successful.\n");
        return true;
    } else {
        printf("Invalid username or password.\n");
        return false;
    }
    
    
}


void loginUserInterface(){
    char username[50];
    char password[50];

    printf("\nEnter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    if (loginUser(username, password)) {
    
    } else {
        printf("Seems you dont have an account, why not create one??");
        registerUser();
    }
}

void displayCandidates(){

    FILE *candidateFile;
    char candidateName[50];
    int candidateVotes;
    int candidateCount = 0;
    
    candidateFile = fopen("candidates.txt", "r");
    if(candidateFile == NULL){
        printf("error opening File. \n");
        return;
    }


    printf("\n List of Presidential Candidates: \n");

    while (fscanf(candidateFile, "%49[^,],%d\n", candidateName, &candidateVotes) == 2)
    {
        printf("%d. %s - Votes: %d\n", ++candidateCount, candidateName, candidateVotes);
    }
    fclose(candidateFile);



}
// bool updateVoteCount(int candidateIndex) {
//     FILE *candidateFile;
//     char candidateName[50];
//     int candidateVotes;
//     int currentIndex = 0;
//     long int pos;

//     // Open the candidate file for reading and writing
//     candidateFile = fopen("candidates.txt", "r+");
//     if (candidateFile == NULL) {
//         printf("Error opening file.\n");
//         return false;
//     }

//     // Update the vote count for the selected candidate
//     while (fscanf(candidateFile, "%49[^,],%d%*c\n", candidateName, &candidateVotes) == 2) {
//         currentIndex++;
//         if (currentIndex == candidateIndex) {
//             // Store the current file position
//             pos = ftell(candidateFile);

//             // Move the file pointer to the beginning of the vote count
//             fseek(candidateFile, pos - sizeof(int), SEEK_SET);
            
//             // Update the vote count
//             fprintf(candidateFile, "%d", candidateVotes + 1);

//             fclose(candidateFile);
//             return true;
//         }
//     }


//     fclose(candidateFile);
//     return false;
// }


bool updateVoteCount(int candidateIndex) {
    FILE *candidateFile;
    struct Candidate candidates[MAX_CANDIDATES];
    int numCandidates = 0;

    // Open the candidate file for reading
    candidateFile = fopen("candidates.txt", "r");
    if (candidateFile == NULL) {
        printf("Error opening file.\n");
        return false;
    }

    // Read candidate data into memory
    while (fscanf(candidateFile, "%49[^,],%d\n", candidates[numCandidates].name, &candidates[numCandidates].votes) == 2) {
        numCandidates++;
    }

    fclose(candidateFile);

    // Check if the candidate index is valid
    if (candidateIndex < 1 || candidateIndex > numCandidates) {
        printf("Invalid candidate index.\n");
        return false;
    }

    // Increment the vote count for the selected candidate
    candidates[candidateIndex - 1].votes++;

    // Open the candidate file for writing
    candidateFile = fopen("candidates.txt", "w");
    if (candidateFile == NULL) {
        printf("Error opening file.\n");
        return false;
    }

    // Write the updated candidate data back to the file
    for (int i = 0; i < numCandidates; i++) {
        fprintf(candidateFile, "%s,%d\n", candidates[i].name, candidates[i].votes);
    }

    fclose(candidateFile);
    return true;
}




void vote(){
    int choice;

    displayCandidates();
    printf("\n Enter the Candidates Nimber you want to vote for");
    scanf("%d", &choice);

    if (updateVoteCount(choice)){
        printf("Thank you for Voting! \n");

    } else{
        printf("inalid Candidate Choice");
    }
    



}

void showResults(){

}