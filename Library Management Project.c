#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Books{
	int book_id;
	char book_name[60];
	char author_name[50];
	int num_book;
	struct Books* next;
};
typedef struct Books Books;
int default_book_id = 1;

Books *books_top = NULL;
Books *books_tail = NULL;



struct Follow{
	int follow_id;
	char follow_name[50];
	Books* follow_booklist;
	struct Follow* next;
	
};
typedef struct Follow Follow;



struct User{
	int user_id;
	char user_password[50];
	char user_name[50];
	int privacy;
	Books* booklist;
	Follow* followlist;
	Follow* followers;
	struct User* next;
	
};
typedef struct User User;
int default_user_id = 1;

User *user_top = NULL;
User *user_tail = NULL;
User *current_user = NULL;


//--------------BOOK PART----------------------------------

void add_book_lib(char name[60], char author[50], int num){
	Books *newBook;
	newBook = (Books*)malloc(sizeof(Books));
	
	newBook->book_id = default_book_id;
	strcpy(newBook->book_name,name);
	strcpy(newBook->author_name,author);
	newBook->num_book = num;
	newBook->next = NULL;
	default_book_id++;
	
	if(books_top == NULL && books_tail == NULL){
		books_top = newBook;
		books_tail = newBook;
	}
	
	else{
		books_tail->next = newBook;
		books_tail = books_tail->next;
	}
	
}


int print_books(){
	
	Books *print_book = books_top;
	
	if(print_book == NULL){
		printf("There is no book in library.\n");
		return 0;
	}
	
	printf("Id    Name\t\tAuthor\t\tTotal\n");
	printf("-----------------------------------------------------------\n");
	while(print_book != NULL){
		printf("[%d]   %s\t\t%s\t\t%d\n",print_book->book_id,print_book->book_name,print_book->author_name,print_book->num_book);
		print_book = print_book->next;
	}
	printf("-----------------------------------------------------------\n");
	return 1;
}


int delete_book_lib(int book_id, int num){
	Books *deleteBook;
	Books *deleteBookPrev;
	
	deleteBookPrev = books_top;
	deleteBook = books_top;
	
	if(deleteBook != NULL){
	
		while(book_id != deleteBook->book_id){
			deleteBook = deleteBook->next;
			if(deleteBook == NULL){
				printf("Invalid book id, book not found...\n");
				return 0;
			}
			
		}
		if(deleteBook->num_book == num){
			printf("\n\n'%d' Books named '%s' have been removed from the library.\n\n\n",num,deleteBook->book_name);
			if(deleteBook == books_top){
				books_top = books_top->next;
			}
			else{
				while(deleteBookPrev->next != deleteBook){
					deleteBookPrev = deleteBookPrev->next;
					if(deleteBookPrev == NULL){
						printf("crash\n");
						return 0;
					}
				}
				if(deleteBook->next == NULL){
					deleteBookPrev->next = NULL;
					books_tail = deleteBookPrev;
				}
				else{
					deleteBookPrev->next = deleteBook->next;
				}
			}
			if(books_top == NULL){
				books_tail = NULL;
			}
			free(deleteBook);
			print_books();
			return 1;
		}
		else if(num > deleteBook->num_book){
			printf("\n\nInvalid number to delete operation...\n\n");
			return 0;
		}
		else{
			printf("\n\n'%d' Books named '%s' have been removed from the library.\n\n\n",num,deleteBook->book_name);
			deleteBook->num_book = deleteBook->num_book-num;
			print_books();
			return 1;
		}
	}
	
}

int print_borrowed(){
	Books *my_book = NULL;
	my_book = current_user->booklist;
	
	if(my_book == NULL){
		printf("There is no book in your book list.\n");
		return 0;
	}
	
	printf("Id    Name\t\tAuthor\t\tTotal\n");
	printf("-----------------------------------------------------------\n");
	
	while(my_book != NULL){
		printf("[%d]   %s\t\t%s\t\t%d\n",my_book->book_id,my_book->book_name,my_book->author_name,my_book->num_book);
		my_book = my_book->next;
	}
	printf("-----------------------------------------------------------\n");
}


int borrowBook(int book_id){
	Books *borrow = NULL;
	Books *borrowed = NULL;
	Books *temp_book = NULL;
	
	borrowed = (Books*)malloc(sizeof(Books));
	borrow = books_top;
	temp_book = current_user->booklist;
	
	if(borrow == NULL){
		printf("There are no books in library...\n");
		return 0;
	}
	while(borrow->book_id != book_id){
		borrow = borrow->next;
		if(borrow == NULL){
			printf("Book not found...\n");
			return 0;
		}
	}
	if(borrow->num_book == 0){
		printf("This book is not available...\n");
		return 0;
	}
	borrowed->book_id = borrow->book_id;
	strcpy(borrowed->book_name,borrow->book_name);
	strcpy(borrowed->author_name,borrow->author_name);
	borrowed->num_book = 1;
	borrowed->next = NULL;
	
	borrow->num_book -=1;
		
	if(current_user->booklist == NULL){
		current_user->booklist = borrowed;
		temp_book = current_user->booklist;
		printf("\n\nYour book list:\n\n");
		print_borrowed();
		return 1;
	}
	else{
		while(temp_book->next != NULL){
			temp_book = temp_book->next;
		}
		temp_book->next = borrowed;
		temp_book = temp_book->next;
		printf("\n\nYour book list:\n\n");
		print_borrowed();
		return 1;
	}
	
}
int returnBook(int givenBookId){
	
	int count = 0,i = 0;

	Books *retBook = NULL;
	Books *retBookPrev = NULL;
	Books *searchBook = NULL;
	
	retBook = current_user->booklist;
	retBookPrev = current_user->booklist;
	searchBook = books_top;
	
	if(retBook == NULL){
		return 0;
	}
	while(givenBookId != retBook->book_id){
		retBook = retBook->next;
		count++;
		if(retBook == NULL){
			printf("Invalid book id.\n");
			return 0;
		}
	}
	
	if(retBook->book_id == givenBookId){
			
		if(count == 0){
			current_user->booklist = current_user->booklist->next;
		}
		else{
			while(i < count-1){
				retBookPrev = retBookPrev->next;
			}
			retBookPrev->next = retBook->next;
		}
		free(retBook);
		print_borrowed();
		
		while(givenBookId != searchBook->book_id){
		searchBook = searchBook->next;
		}
		searchBook->num_book+= 1;
		return 1;
	}
}

//--------------USER PART-----------------------------

int registration(char name[50], char password[11], int privacy){

	User* newUser;
	newUser = (User*)malloc(sizeof(User));
	
	newUser->user_id = default_user_id;
	strcpy(newUser->user_name,name);
	strcpy(newUser->user_password,password);
	newUser->privacy = privacy;
	newUser->booklist = NULL;
	newUser->followlist = NULL;
	newUser->next = NULL;
	newUser->followers = NULL;
	
	default_user_id++;
	
	if(user_top == NULL && user_tail == NULL){
		
		user_top = newUser;
		user_tail= newUser;
	}
	else{
		user_tail->next = newUser;
		user_tail = user_tail->next;
	}
	
}

int follow_someone(int given_id){
	
	User* search_user = NULL;
	search_user = user_top;
	
	if(given_id != current_user->user_id){
		while(search_user->user_id != given_id){
			search_user = search_user->next;
			if(search_user == NULL){
				printf("\nUser not found.\n");
				return 0;
			}
		}
	}
	else{
		printf("\nYou can not follow yourself.\n");
		return 0;
	}
	
	Follow* temp_followed = NULL;
	temp_followed = (Follow*)malloc(sizeof(Follow));
	
	strcpy(temp_followed->follow_name,search_user->user_name);
	temp_followed->follow_id = given_id;
	temp_followed->follow_booklist = search_user->booklist;
	temp_followed->next = NULL;
	
	Follow* current_followlist = current_user->followlist;
	Follow* checker = current_user->followlist;
	
	
	if(current_followlist == NULL){
		
		current_user->followlist = temp_followed;
		current_followlist = current_user->followlist;
		checker = current_user->followlist;
	}
	else{
		
		while(checker->follow_id != given_id){
			checker = checker->next;
			if(checker == NULL){
				checker = current_followlist ;
				break;
			}
		}
		if(checker->follow_id == given_id){
			printf("\nYou are already following %s\n",search_user->user_name);
			return 0;
		}
		
		while(current_followlist->next != NULL){
			current_followlist = current_followlist->next;
		}
		current_followlist->next = temp_followed;
		current_followlist = current_followlist->next;
	}
	
	Follow* add_follower = search_user->followers;
	Follow* temp_follower = NULL;
	
	temp_follower = (Follow*)malloc(sizeof(Follow));
	
	temp_follower->follow_id = current_user->user_id;
	strcpy(temp_follower->follow_name,current_user->user_name);
	temp_follower->follow_booklist = current_user->booklist;
	temp_follower->next = NULL;
	
	if(search_user->followers == NULL){
		
		search_user->followers = temp_follower;
		add_follower = search_user->followlist;
	}
	else{
		while(add_follower->next != NULL){
			add_follower = add_follower->next;
		}
		add_follower->next = temp_follower;
		add_follower = add_follower->next;
	}
	return 1;
}


int unf_someone(int given_id){
	
	Follow* search_followed = current_user->followlist;
	Follow* prev_search_followed = current_user->followlist;
	
	if(search_followed == NULL){
		return 0;
	}
	
	while(search_followed->follow_id != given_id){
		search_followed = search_followed->next;
		if(search_followed == NULL){
			printf("\nUser not found.\n");
			return 0;
		}
	}
	if(search_followed != current_user->followlist){
		while(prev_search_followed->next != search_followed){
			prev_search_followed = prev_search_followed->next;
			if(prev_search_followed == NULL){
				printf("\nCrash 1\n");
				return 0;
			}
		}
	}
	
	
	if(search_followed == current_user->followlist && search_followed->next == NULL){
		current_user->followlist = NULL;
	}
	else if(search_followed == current_user->followlist && search_followed->next != NULL){
		current_user->followlist = search_followed->next;
	}
	else if(search_followed != current_user->followlist && search_followed->next == NULL){
		prev_search_followed->next = NULL;
	}
	else if(search_followed != current_user->followlist && search_followed->next != NULL){
		prev_search_followed->next = search_followed->next;
	}
		
	else if(prev_search_followed == current_user->followlist && search_followed->next != NULL){
		current_user->followlist = search_followed->next;
		prev_search_followed = current_user->followlist;	
	}
	else {
		prev_search_followed->next = search_followed->next;
	}
	
	User* unf_person = user_top;
	
	
	while(unf_person->user_id != given_id){
		unf_person = unf_person->next;
		if(unf_person == NULL){
			printf("\nCrash 2");
			return 0;
		}
	}

	Follow* delete_follower = unf_person->followers;
	Follow* prev_delete_follower = unf_person->followers;	
	
	while(delete_follower->follow_id != current_user->user_id){
		delete_follower = delete_follower->next;
		if(delete_follower == NULL){
			printf("\nCrash 3\n");
			return 0;
		}
	}
	
	if(delete_follower != unf_person->followers){
		while(prev_delete_follower->next != delete_follower){
			prev_delete_follower = prev_delete_follower->next;
			if(prev_delete_follower == NULL){
				printf("\nCrash 4\n");
				return 0;
			}
		}
	}
	if(delete_follower == unf_person->followers && delete_follower->next == NULL){
		unf_person->followers = NULL;
	}
	else if(delete_follower == unf_person->followers && delete_follower->next != NULL){
		unf_person->followers = delete_follower->next;
	}
	else if(delete_follower != unf_person->followers && delete_follower->next == NULL){
		prev_delete_follower->next = NULL;
	}
	else if(delete_follower != unf_person->followers && delete_follower->next != NULL){
		prev_delete_follower->next = delete_follower->next;
	}
		
	else if(prev_delete_follower == unf_person->followers && delete_follower->next != NULL){
		unf_person->followers = delete_follower->next;
		prev_delete_follower = unf_person->followers;	
	}
	else {
		prev_search_followed->next = search_followed->next;
	}
	
	
	free(search_followed);
	free(delete_follower);
	return 1;
	
}

void print_followed_books(given_id){
	
	User* search_followed = user_top;
	Books* followed_book = NULL;
	Follow* search_followlist = current_user->followlist;
	
	if(search_followlist == NULL){
		return;
	}
	else{
		while(search_followlist->follow_id != given_id){
			search_followlist = search_followlist->next;
			if(search_followlist == NULL){
				printf("\nUser not found.\n");
				return;
			}
		}
	}
	
	
	while(search_followed->user_id != given_id){
		search_followed = search_followed->next;
		if(search_followed == NULL){
			printf("\nUser not found.\n");
			return;
		}
	}
	if(search_followed->privacy == 1){// private
		printf("\nThe user has private account..\n");
		return;
	}
	else{
		followed_book = search_followed->booklist;
		
		if(followed_book == NULL){
			printf("There is no book in book list.\n");
			return;
		}
	
		printf("Id    Name\t\tAuthor\t\tTotal\n");
		printf("-----------------------------------------------------------\n");
	
		while(followed_book != NULL){
			printf("[%d]   %s\t\t%s\t\t%d\n",followed_book->book_id,followed_book->book_name,followed_book->author_name,followed_book->num_book);
			followed_book = followed_book->next;
		}
		printf("-----------------------------------------------------------\n");
		
	}
	
}
void print_followed(){
	
	Follow* temp_followed;
	temp_followed = current_user->followlist;
	if(current_user->followlist == NULL){
		printf("\nYou are not following anyone.\n");
		return;
	}
	printf("-----------------------------\n");
	printf("     People You Follow \n");
	printf("-----------------------------\n");
	while(temp_followed != NULL){
		printf("[%d]  %s\n",temp_followed->follow_id,temp_followed->follow_name);
		temp_followed = temp_followed->next;
	}
	printf("-----------------------------\n");
}

void print_followers(){
	
	Follow* temp_followers;
	temp_followers = current_user->followers;
	if(current_user->followers == NULL){
		printf("\nNo one is following you.\n");
		return;
	}
	printf("-----------------------------\n");
	printf("          Followers \n");
	printf("-----------------------------\n");
	while(temp_followers != NULL){
		printf("[%d]  %s\n",temp_followers->follow_id,temp_followers->follow_name);
		temp_followers = temp_followers->next;
	}
	printf("-----------------------------\n");
}

void print_users(){
	
	User *print_user = user_top;
	
	if(print_user == NULL){
		printf("\n** WARNING **\n\nThere is no user in this system...\n");
		return;
	}
	
	printf("-------------------------------------------------------\n");
	printf("id  Userame\n");
	printf("--- -------\n");
	
	while(print_user != NULL){
		printf("[%d] %s\n",print_user->user_id,print_user->user_name);
		print_user = print_user->next;
	}
	printf("\n-------------------------------------------------------\n");
}

int delete_users(int id){
	int count = 1;
	User *delete_user = NULL;
	User *delete_prev = NULL;
	
	delete_user = user_top;
	delete_prev = user_top;
	
	if(delete_user != NULL){
		while(delete_user->user_id != id){
			delete_user = delete_user->next;
			if(delete_user == NULL){
				printf("Invalid user id, user not found...\n");
				return 0;
			}
		}
	
	if(delete_user == user_top){
		user_top = user_top->next;
	}
	else{
		while(delete_prev->next != delete_user){
			delete_prev = delete_prev->next;
			if(delete_prev == NULL){
				printf("crash\n");
				return 0;
			}
		}
		if(delete_user->next == NULL){
			delete_prev->next = NULL;
			user_tail = delete_prev;
		}
		else{
			delete_prev->next = delete_user->next;
		}
	}
	if(user_top == NULL){
		user_tail = NULL;
	}
	
	
	free(delete_user);
	print_users();
	return 1;
	}
}


int login(char given_name[50],char given_password[50]){

	User* temp;
	temp = user_top;
	
	if(strcmp(given_name,"admin") == 0 && strcmp(given_password,"4321") == 0){
		return 2;
	}
	if(temp == NULL){
		printf("User not found\n");
		return 0;
	}
	else{
		while(strcmp(given_name,temp->user_name) != 0 ){
			
			temp = temp->next;
			if(temp == NULL){
				printf("User not found\n");
				return 0;	
			}
		}
	}
	
	if(strcmp(given_name,temp->user_name) == 0){
		if(strcmp(given_password,temp->user_password) == 0){
			printf("Successfully logged in...\n");
			current_user = temp;
			return 1;
		}
		else{
			printf("User not found\n");
			return 0;
		}
	}
}
//----------------------------MENU PART----------------------

void main_menu();

void admin_menu(){
	int temp_id;
    int num;
    char name[60];
    char author[50];
    User *temp = NULL;
    temp = user_top;
    
	printf("***********************************\n"); 
    printf("           WELCOME ADMIN \n"); 
	printf("***********************************\n\n"); 
    printf("[1] Delete User\n");
    printf("[2] Delete Book From Library\n");
    printf("[3] Add Book to Library\n");
    printf("[4] List Users\n");
    printf("[5] Quit\n");
    
    int choice;
    printf("\nYour choice:");
    scanf("%d",&choice);
    printf("\n");
    
    switch(choice){
    	case 1:
    		print_users();
    		printf("Enter an user user id:");
    		scanf("%d",&temp_id);
    		delete_users(temp_id);
    		admin_menu();
    		break;
    	case 2:
    		print_books();
    		printf("Enter book id:");
    		scanf("%d",&temp_id);
    		printf("How many books do you want to delete? :");
    		scanf("%d",&num);
    		delete_book_lib(temp_id,num);
    		admin_menu();
    		break;
    	case 3:
    		print_books();
    		printf("\n");
    		printf("**Use '_' instead of spaces**\n");
    		printf("Name of the book:");
    		scanf("%",name);
    		printf("\n");
    		printf("Author of the book:");
    		scanf("%s",author);
    		printf("\n");
    		printf("How many books do you want to add:");
    		scanf("%d",&num);
    		add_book_lib(name,author,num);
    		printf("\n");
    		print_books();
    		admin_menu();
    		break;
    	case 4:
    		print_users();
    		admin_menu();
    		break;
    	case 5:
    		printf("\nHave a nice day...\n");
    		main_menu();
    		break;
    	default:
    		printf("Invalid choice..\n\n");
    		admin_menu();
    		break;
	}
}

void user_menu(){
	int input;
	int temp_id;
	printf("\n---------------------\n");
	printf("--- Welcome %s ---\n",current_user->user_name);
	printf("---------------------\n");
	printf("[1] My Books\n");
	printf("[2] List Library Books\n");
	printf("[3] Book Return Process\n");
	printf("[4] Borrow a Book\n");
	printf("[5] People You Follow\n");
	printf("[6] Follow Someone\n");
	printf("[7] Show Followers \n");
	printf("[8] Unfollow Someone\n");
	printf("[9] Show Books of People I Follow\n");
	printf("[10] Quit\n");
	
	printf("Your choice:");
	scanf("%d",&input);
	
	switch(input){
		case 1:
			print_borrowed();
			user_menu();
			break;
		case 2:
			print_books();
			user_menu();
			break;
		case 3:
			print_borrowed();
			printf("Enter a book id:");
			scanf("%d",&temp_id);
			returnBook(temp_id);
			user_menu();
			break;
		case 4:
			print_books();
			printf("\nEnter a book id:");
			scanf("%d",&temp_id);
			borrowBook(temp_id);
			user_menu();
			break;
		case 5:
			print_followed();
			user_menu();
			break;
		case 6:
			print_users();
			printf("Please enter an id to follow someone:");
			scanf("%d",&temp_id);
			follow_someone(temp_id);
			print_followed();
			user_menu();
			break;
		case 7:
			print_followers();
			user_menu();
			break;
		case 8:
			print_followed();
			printf("Who do you want to unfollow? id:");
			scanf("%d",&temp_id);
			unf_someone(temp_id);
			print_followed();
			user_menu();
			break;
		case 9:
			print_followed();
			printf("\nPlease enter an id:");
			scanf("%d",&temp_id);
			print_followed_books(temp_id);
			user_menu();
			break;
		case 10:
			printf("\nHave a nice day %s!\n",current_user->user_name);
			main_menu();
			break;
		default:
			printf("Invalid choice...\n");
			user_menu();
			break;
	}
}
void main_menu(){
	int input;
	char userName[50];
	char userPassword[50];
	int given_privacy;
	
	char given_name[50];
	char given_password[50];
	
	
	printf("********************************\n");
	printf("*****  Welcome to Library  *****\n");
	printf("********************************\n\n");
	printf("[1] Login\n");
	printf("[2] Register\n");
	printf("[3] Quit\n");
	printf("Your Choice:");
	scanf("%d",&input);
	
	if(input == 1){
		printf("Enter username: ");
		scanf("%s",given_name);

		printf("Enter Password: ");
		scanf("%s",given_password);
		
		switch (login(given_name,given_password)){
			case 1:
				user_menu();
				break;
			case 2:
				admin_menu();
				break;
			default:
				main_menu();
		}
		
	}
	else if(input == 2){
		printf("Enter an username:");
		scanf("%s",userName);
			
		printf("Enter a password:");
		scanf("%s",userPassword);	
		printf("Do you want to create private account? 1 for yes, 0 for no:");
		scanf("%d",&given_privacy);
		
		registration(userName,userPassword,0);
		main_menu();
	}
	else if(input == 3){
		printf("\n\nHave a nice day..\n");
		
	}
}

int main() {
	
	add_book_lib("test1","author1",3);
	add_book_lib("test2","author1",9);
	add_book_lib("test3","author2",9);


	registration("user1","1234",0);
	registration("user2","1234",1);

	main_menu();
	return 0;
}
