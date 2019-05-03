#include <ncurses.h>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>
/* 80 wide by 24 tall */

using namespace std;

int binary_search(string dictionary[], int left, int right, string key){
  if(right >= left){
    int middle = left + (right - left) / 2;

    if(dictionary[middle].compare(key) == 0){
      return middle;
    }

    if(dictionary[middle].compare(key) > 0){
      return binary_search(dictionary, left, middle - 1, key);
    }

    return binary_search(dictionary, middle + 1, right, key);
  }

  return -1;
}

int erase_punct_from_string(string *str){
  string::iterator it;
  int num_bad_punct = 0;

  for(it = (*str).begin(); it < (*str).end(); it++){
    if(*it == '.' || *it == ',' || *it == '?' || *it == '!' || *it == '"' ||
       *it == ';' || *it == ':' || *it == '(' || *it == ')' || *it == '-'){
      num_bad_punct ++;
      (*str).erase(it);
    }
  }
  return num_bad_punct;
}

int main(){
  initscr();
  if(has_colors() == false){
    endwin();
    printf("Your terminal does not support colors. Get Wrecked.\n");
    exit(1);
  }
  raw();
  curs_set(1);
  keypad(stdscr, TRUE);
  noecho();

  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);

  cout << "Starting spellchecker..." << endl;

  string dictionary[45456]; 

  ifstream wordsFile("words.txt");
  string temp;

  int count = 0;
  
  while(getline(wordsFile, temp)){
    temp = temp.substr(0, temp.size() - 1);
    //temp.erase(std::find(temp.begin(), temp.end(), '\0'), temp.end());

    for(int i = 0; i < (int)temp.length(); i ++){
      temp[i] = tolower(temp[i]);
    }

    dictionary[count] = temp;
    //cout << temp << "-" << endl;
    count++;
  }


  int end = sizeof(dictionary) / sizeof(dictionary[0]);

  //words.txt file is not already sorted
  sort(dictionary, dictionary + end);
  
  wordsFile.close();


  char ch;

  int x = 0;
  int y = 0;

  mvprintw(23, 0, "Start typing to spellcheck. Press escape to quit.");
  move(y, x);

  string current_word;
  //mvaddstr(5, 0, "---------------------------------------------------");
  
  while(ch != 27){
    ch = getch();
    if(ch != 7){//if not backspace
      
      if(ch == ' '){
	//mvaddstr(5, 0, current_word.c_str());
	//mvaddstr(5, current_word.size(), ".");

	int extra_characters = erase_punct_from_string(&current_word);
	
	if(binary_search(dictionary, 0, end - 1, current_word) == -1){
	  
	  attron(COLOR_PAIR(1));
	  for(uint32_t i = 0; i <= current_word.size() + extra_characters; i ++)
	  {
	    char c = mvinch(y, x - i);
	    mvaddch(y, x - i, c);
	  }
	  attroff(COLOR_PAIR(1));
	}
	current_word = "";
      }

      
      if(x == 80){
	x = 0;
	y += 1;
      }
      mvaddch(y, x, ch);
      x += 1;
      if(ch != ' '){
	current_word.push_back(tolower(ch));
      }
      
    }else if(ch == 7){ 
      if(x == 0){
	if(y != 0){
	  y -= 1;
	  x = 80;
	  mvaddch(y, x, ' ');
	  move(y, x);
	  current_word = current_word.substr(0, current_word.size() - 1);
	}
      }else{ 
	x -= 1;
	mvaddch(y, x, ' ');
	move(y, x);
	current_word = current_word.substr(0, current_word.size() - 1);
      }
    }
  }


  endwin();

  return 0;
}
