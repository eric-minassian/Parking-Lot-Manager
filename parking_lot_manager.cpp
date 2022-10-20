#include <iostream>
#include <string>

using namespace std;

void print_menu() {
  cout
      << "----------\n|  MENU  |\n----------\nPlease use one of the following "
         "commands:\n  - (CT) check time\n  - (AT) advance time\n  - (SS) show "
         "spots\n  - (TS) take spot\n  - (CP) check price\n  - (LS) leave "
         "spot\n "
         " - (CS) check stats\n  - (EX) exit the program\n";
}

void split(string &user_input, string command[]) {
  unsigned short count = 0;

  for (int i = 0; i < 4; i++) {
    command[i] = "";
  }
  while (user_input.find(' ') != string::npos) {
    command[count] = user_input.substr(0, user_input.find(' '));
    user_input.erase(0, user_input.find(' ') + 1);
    count++;
  }
  command[count] = user_input;
}

void CT(unsigned int time) {
  cout << "We have been open for " << time << " minutes!\n\n";
}

void AT(unsigned int &time, string param_1, string param_2) {
  if (param_2.empty()) {
    time += stoi(param_1);
  } else {
    time += (stoi(param_1) * 60) + stoi(param_2);
  }
  cout << '\n';
}

void SS(string parking_lot[][6], string option) {
  if (option == "O" || option == "A") {
    cout << "Open spots:\n";
    for (int r = 0; r < 4; r++) {
      cout << "Row " << r + 1 << ":";
      for (int c = 0; c < 6; c++) {
        if (parking_lot[r][c].empty()) {
          cout << ' ' << (r * 6 + c + 1);
        }
      }
      cout << '\n';
    }
    cout << '\n';
  }

  if (option == "T" || option == "A") {
    cout << "Taken spots:\n";
    for (int r = 0; r < 4; r++) {
      cout << "Row " << r + 1 << ":";
      for (int c = 0; c < 6; c++) {
        if (!parking_lot[r][c].empty()) {
          cout << ' ' << (r * 6 + c + 1);
        }
      }
      cout << '\n';
    }
    cout << '\n';
  }
}

void TS(string parking_lot[][6], unsigned int parking_time[][6],
        unsigned short row, unsigned short column, string plate,
        unsigned int time, unsigned int &total_parked,
        unsigned int &currently_parked) {
  for (int r = (row - 1); r < 4; r++) {
    for (int c = (column - 1); c < 6; c++) {
      if (parking_lot[r][c].empty()) {
        cout << "Parked " << plate << " in spot " << ((r * 6) + c + 1)
             << ".\n\n";
        parking_lot[r][c] = plate;
        parking_time[r][c] = time;
        total_parked += 1;
        currently_parked += 1;
        return;
      }
    }
    column = 1;
  }
  cout << "Sorry, you've passed all the open spots already!\n\n";
}

void CP(string parking_lot[][6], unsigned int parking_time[][6],
        unsigned int time, string param_1, string param_2) {
  unsigned int row, column, time_spent, price;

  if (param_2.empty()) {
    for (short r = 0; r < 4; r++) {
      for (short c = 0; c < 6; c++) {
        if (parking_lot[r][c] == param_1) {
          row = r;
          column = c;
          break;
        }
      }
    }
  } else {
    row = stoi(param_1) - 1;
    column = stoi(param_2) - 1;
  }
  time_spent = time - parking_time[row][column];
  cout << "Vehicle " << parking_lot[row][column] << " has been in "
       << (row * 6 + column + 1) << " for " << time_spent << " minutes.\n";

  if (time_spent > 360) {
    time_spent -= 60;
  } else if (time_spent > 300) {
    time_spent -= (time_spent - 300);
  }
  price = (int)(((float)time_spent / 60) * 10);

  cout << "It owes $" << price << " at the moment.\n\n";
}

void LS(string parking_lot[][6], unsigned int parking_time[][6],
        unsigned int time, string param_1, string param_2,
        unsigned int &currently_parked, unsigned int &total_charged) {
  unsigned int row, column, time_spent, price;

  if (param_2.empty()) {
    for (short r = 0; r < 4; r++) {
      for (short c = 0; c < 6; c++) {
        if (parking_lot[r][c] == param_1) {
          row = r;
          column = c;
          break;
        }
      }
    }
  } else {
    row = stoi(param_1) - 1;
    column = stoi(param_2) - 1;
  }
  time_spent = time - parking_time[row][column];

  if (time_spent > 360) {
    time_spent -= 60;
  } else if (time_spent > 300) {
    time_spent -= (time_spent - 300);
  }
  price = (int)(((float)time_spent / 60) * 10);

  cout << "Vehicle " << parking_lot[row][column] << " left spot "
       << (row * 6 + column + 1) << ".\n";
  cout << "It was parked there for " << (time - parking_time[row][column])
       << " minutes and paid $" << price << ".\n\n";

  parking_lot[row][column] = "";
  parking_time[row][column] = 0;
  currently_parked -= 1;
  total_charged += price;
}

int main() {
  string user_input, command[4], parking_lot[4][6];
  unsigned int parking_time[4][6], time = 0, total_parked = 0,
                                   currently_parked = 0, total_charged = 0,
                                   average_charge = 0;

  // Say introduction
  cout << "----------\nHello! Welcome to the parking lot manager.\nI'll help "
          "you find a spot!\n----------\n\n";

  // Main Menu
  do {
    print_menu();
    getline(cin, user_input);
    split(user_input, command);

    // Main Menu Functions
    if (command[0] == "CT") {
      CT(time);
    } else if (command[0] == "AT") {
      AT(time, command[1], command[2]);
    } else if (command[0] == "SS") {
      SS(parking_lot, command[1]);
    } else if (command[0] == "TS") {
      TS(parking_lot, parking_time, stoi(command[1]), stoi(command[2]),
         command[3], time, total_parked, currently_parked);
    } else if (command[0] == "CP") {
      CP(parking_lot, parking_time, time, command[1], command[2]);
    } else if (command[0] == "LS") {
      LS(parking_lot, parking_time, time, command[1], command[2],
         currently_parked, total_charged);
    } else if (command[0] == "CS") {
      cout << "Total vehicles parked: " << total_parked << '\n';
      cout << "Vehicles currently parked: " << currently_parked << '\n';
      cout << "Total amount charged: $" << total_charged << '\n';
      if (total_parked == currently_parked) {
        average_charge = 0;
      } else {
        average_charge = total_charged / (total_parked - currently_parked);
      }
      cout << "Average charge per vehicle: $" << average_charge << "\n\n";
    } else if (command[0] != "EX") {
      cout << "Sorry, that command is invalid.\n\n";
    }

  } while (command[0] != "EX");

  cout << "Goodbye!\n";

  return 0;
}