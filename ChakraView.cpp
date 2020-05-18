/*
 * ChakraView.cpp
 *
 *  Created on: 24-Jul-2018
 * 	Updated on: 31-Mar-2019
 *      Author: Sudhanshu Bhoi
 */

#include <iostream>
#include <string>
#include <locale>
using namespace std;

class Location {
private:
	int row_index;
	int column_index;

public:
	Location(int ri = -1, int ci = -1) {
		row_index = ri;
		column_index = ci;
	}

	bool operator == (Location locB) {
		return (this->row_index == locB.row_index && this->column_index == locB.column_index);
	}

	friend class Pawn;
	friend class Mantri;
	friend class Soldier;
	friend class BoardGame;
	friend class Player;

	friend ostream & operator << (ostream &, const Location &);
};

ostream & operator << (ostream & os, const Location & loc) {
	os << "\n get_location: R(" << loc.row_index << "), C(" << loc.column_index << ")";
	return os;
}

class Pawn {
protected:
	string colour;
	Location location;
	int index;
	bool eliminated;

public:
	Pawn(const string & colour, Location location, int index) {
		this->colour = colour;
		this->location = location;
		this->index = index;
		eliminated = false;
	}

	Pawn() {
		this->colour = "---";
		this->location = Location(-1, -1);
		this->index = -1;
		this->eliminated = false;
	}

	Location get_location() const {
		return location;
	}

	Location change_location(Location move_to) {
		this->location = move_to;
		return location;
	}

	int get_index() const {
		return index;
	}

	void change_eliminated(bool b) {
		this->eliminated = b;
	}

	bool is_empty() const {
		return this->colour == "---";
	}

	virtual bool is_white() const {
		return false;
	}

	virtual bool is_black() const {
		return false;
	}

	virtual bool is_mantri() const {
		return false;
	}

	virtual bool is_soldier() const {
		return false;
	}

	virtual bool is_valid_move(Location) const{
		return false;
	}

	bool is_eliminated() const {
		return this->eliminated;
	}

	virtual ~Pawn() { }

	friend ostream & operator << (ostream &, const Pawn &);
};

ostream & operator << (ostream & os, const Pawn & pawn) {
	if (pawn.colour != "---") {
		os << pawn.colour[0];
		if (pawn.index < 9)
			os << "0";
		os << pawn.index + 1;
	}
	else
		os << pawn.colour;
	return os;
}

class Mantri : public Pawn {
public:
	Mantri(const char* colour, Location location, int index) : Pawn(colour, location, index) { }
	Mantri() : Pawn() { }

	bool is_white() const {
		return this->colour == "WHITE";
	}

	bool is_black() const {
		return this->colour == "BLACK";
	}

	bool is_mantri() const {
		return true;
	}

	bool is_valid_move(Location move_to) const {
		if (move_to.column_index >= this->location.column_index - 1 && move_to.column_index <= this->location.column_index + 1) {
			if (this->colour[0] == 'W') {
				if (move_to.row_index == this->location.row_index + 1 || move_to.row_index == this->location.row_index)
					return true;
				else if (move_to.column_index == this->location.column_index && move_to.row_index == this->location.row_index + 2)
					return true;
			}

			else if (this->colour[0] == 'B') {
				if (move_to.row_index == this->location.row_index - 1 || move_to.row_index == this->location.row_index)
					return true;
				else if (move_to.column_index == this->location.column_index && move_to.row_index == this->location.row_index - 2)
					return true;
			}
		}

		else if (move_to.row_index == this->location.row_index && (move_to.column_index == this->location.column_index + 2 ||
				move_to.column_index == this->location.column_index - 2))
			return true;

		return false;
	}


};

class Soldier : public Pawn {
public:
	Soldier(const char* colour, Location location, int index) : Pawn(colour, location, index) { }
	Soldier() : Pawn() { }

	bool is_white() const {
		return this->colour == "white";
	}

	bool is_black() const {
		return this->colour == "black";
	}

	bool is_soldier() const {
		return true;
	}

	bool is_valid_move(Location move_to) const {
		if (move_to.column_index >= this->location.column_index - 1 && move_to.column_index <= this->location.column_index + 1) {
			if (this->colour[0] == 'w') {
				if (move_to.row_index == this->location.row_index + 1 && move_to.column_index != this->location.column_index)
					return true;
				else if (move_to.column_index == this->location.column_index && move_to.row_index == this->location.row_index + 2)
					return true;
			}

			else if (this->colour[0] == 'b') {
				if (move_to.row_index == this->location.row_index - 1 && move_to.column_index != this->location.column_index)
					return true;
				else if (move_to.column_index == this->location.column_index && move_to.row_index == this->location.row_index - 2)
					return true;
			}
		}

		else if (move_to.row_index == this->location.row_index && (move_to.column_index == this->location.column_index + 2 ||
				move_to.column_index == this->location.column_index - 2))
			return true;

		return false;
	}
};


class BoardGame {
private:
	int size;
	int number_of_white_soldiers;
	int number_of_white_mantri;
	int number_of_black_soldiers;
	int number_of_black_mantri;
	Mantri* white_mantris;
	Soldier* white_soldiers;
	Mantri* black_mantris;
	Soldier* black_soldiers;
	Pawn*** board;

public:
	Pawn clear;
	BoardGame(int size_board = 7, int no_soldiers = 14, int no_mantris = 7) {
		this->size = size_board;
		this->number_of_white_soldiers = no_soldiers;
		this->number_of_white_mantri = no_mantris;
		this->number_of_black_soldiers = no_soldiers;
		this->number_of_black_mantri = no_mantris;
		this->white_soldiers = new Soldier[this->number_of_white_soldiers];
		this->white_mantris = new Mantri[this->number_of_white_mantri];
		this->black_soldiers = new Soldier[this->number_of_black_soldiers];
		this->black_mantris = new Mantri[this->number_of_black_mantri];

		for (int i = 0; i < this->size; i++) {
            this->white_mantris[i] = Mantri("WHITE", Location(0, i), i);
            this->white_soldiers[i] = Soldier("white", Location(1, i), i);
            this->white_soldiers[i+this->size] = Soldier("white", Location(2, i), i+this->size);
            this->black_soldiers[i+this->size] = Soldier("black", Location(this->size-3, i), i+this->size);
            this->black_soldiers[i] = Soldier("black", Location(this->size-2, i), i);
            this->black_mantris[i] = Mantri("BLACK", Location(this->size-1, i), i);
		}

		this->board = new Pawn**[this->size];
		for (int i = 0; i < this->size; i++) {
			this->board[i] = new Pawn*[this->size];
		}

		//this->initialize_board();
		this->update_current_board();
	}

	int get_size() const {
		return this->size;
	}

	bool is_empty(Location loc) {
		return this->board[loc.row_index][loc.column_index]->is_empty();
	}

	bool is_mantri(Location loc) {
		return this->board[loc.row_index][loc.column_index]->is_mantri();
	}

	bool is_soldier(Location loc) {
		return this->board[loc.row_index][loc.column_index]->is_soldier();
	}

	bool is_black(Location loc) {
		return this->board[loc.row_index][loc.column_index]->is_black();
	}

	bool is_white(Location loc) {
		return this->board[loc.row_index][loc.column_index]->is_white();
	}

	Pawn* get_pawn(Location loc) const {
		return this->board[loc.row_index][loc.column_index];
	}

	Mantri* get_white_mantris() const {
		return this->white_mantris;
	}

	Soldier* get_white_soldiers() const {
		return this->white_soldiers;
	}

	Mantri* get_black_mantris() const {
		return this->black_mantris;
	}

	Soldier* get_black_soldiers() const {
		return this->black_soldiers;
	}

	void initialize_board() {
		for (int row = 0; row <  this->size; row++) {
			for (int col = 0; col < this->size; col++) {
				board[row][col] = &this->clear;
			}
		}
	}

	void update_current_board() {
		this->initialize_board();

		for (int i = 0; i < this->size; i++) {
			if (not this->white_mantris[i].is_eliminated())
				board[this->white_mantris[i].get_location().row_index][this->white_mantris[i].get_location().column_index] = &this->white_mantris[i];
			if (not this->white_soldiers[i].is_eliminated())
				board[this->white_soldiers[i].get_location().row_index][this->white_soldiers[i].get_location().column_index] = &this->white_soldiers[i];
			if (not this->white_soldiers[i+this->size].is_eliminated())
				board[this->white_soldiers[i+this->size].get_location().row_index][this->white_soldiers[i+this->size].get_location().column_index] = &this->white_soldiers[i+this->size];
			if (not this->black_soldiers[i+this->size].is_eliminated())
				board[this->black_soldiers[i+this->size].get_location().row_index][this->black_soldiers[i+this->size].get_location().column_index] = &this->black_soldiers[i+this->size];
			if (not this->black_soldiers[i].is_eliminated())
				board[this->black_soldiers[i].get_location().row_index][this->black_soldiers[i].get_location().column_index] = &this->black_soldiers[i];
			if (not this->black_mantris[i].is_eliminated())
				board[this->black_mantris[i].get_location().row_index][this->black_mantris[i].get_location().column_index] = &this->black_mantris[i];
		}
	}

	void display_black_board() {
		cout << "\n";
		for (int i = 0; i < this->size; i++)
			cout << "\t" << i + 1;
		cout << "\n";
		for (int i = 0; i < this->size; i++) cout << "\t-";

		for (int row = 0; row < this->size; row++) {
			cout << "\n" << row + 1 << " | ";
			for (int col = 0; col < this->size; col++) {
					cout << "\t" << *board[row][col];
			}
		}
	}

	void display_white_board() {
		for (int row = this->size - 1; row >= 0; row--) {
			cout << "\n" << row + 1 << " | ";
			for (int col = 0; col < this->size; col++) {
				cout << "\t" << *board[row][col];
			}
		}

		cout << "\n";
		for (int i = 0; i < this->size; i++) cout << "\t-";
		cout << "\n";
		for (int i = 0; i < this->size; i++)
			cout << "\t" << i + 1;
	}


	Pawn & parse_pawn(char pawn_type, int index) {
		index = index - 1;
		switch (pawn_type) {
		case 'W':
			return this->white_mantris[index];
		case 'w':
			return this->white_soldiers[index];
		case 'B':
			return this->black_mantris[index];
		case 'b':
			return this->black_soldiers[index];
		default:
			throw "Invalid Pawn!";
		}
	}

	template <class MS>
	bool make_move(MS* pawn, Location move_to) {
		if (pawn->change_location(move_to) == move_to) {
			this->update_current_board();
			return true;
		}
		return false;
	}

	bool game_over() {
		for (int col = 0; col < this->size; col++)
			if ((this->board[0][col]->is_black() && this->board[0][col]->is_mantri()) ||
					(this->board[6][col]->is_white() && this->board[6][col]->is_mantri()))
				return true;
		return false;
	}

	// Destructor
	~BoardGame() {
		delete[] this->white_mantris;
		delete[] this->white_soldiers;
		delete[] this->black_soldiers;
		delete[] this->black_mantris;

		for (int i = 0; i < this->size; i++) {
			delete[] this->board[i];
		}
		delete[] this->board;
	}
};

class Eliminated {
private:
	Pawn* mantri_soldier;
	Eliminated* next_mantri_soldier;

public:
	explicit Eliminated(Pawn* mantri_soldier) {
		this->mantri_soldier = mantri_soldier;
		this->next_mantri_soldier = NULL;
	}

	friend class EliminatedContainer;
};

class EliminatedContainer {
private:
	Eliminated *first_mantri;
	Eliminated *first_soldier;

public:
	EliminatedContainer() {
		first_mantri = NULL;
		first_soldier = NULL;
	}

	void add_to_container(Pawn* e_pawn) {
		if (e_pawn->is_mantri()) {
			if (first_mantri == NULL) {
				first_mantri = new Eliminated(e_pawn);
			}
			else {
				Eliminated *current_mantri = first_mantri;
				while (current_mantri->next_mantri_soldier)
					current_mantri = current_mantri->next_mantri_soldier;
				current_mantri->next_mantri_soldier = new Eliminated(e_pawn);
			}
		}
		else if (e_pawn->is_soldier()) {
			if (first_soldier == NULL) {
				first_soldier = new Eliminated(e_pawn);
			}
			else {
				Eliminated *current_soldier = first_soldier;
				while (current_soldier->next_mantri_soldier)
					current_soldier = current_soldier->next_mantri_soldier;
				current_soldier->next_mantri_soldier = new Eliminated(e_pawn);
			}
		}
	}

	bool retrieve_mantri(Pawn* & retrieved) {
		if (not first_mantri)
			return false;

		retrieved = first_mantri->mantri_soldier;
		Eliminated* first = first_mantri;
		first_mantri = first_mantri->next_mantri_soldier;
		delete first;
		return true;
	}

	~EliminatedContainer() {
		Eliminated *current_mantri = this->first_mantri;
		Eliminated *next_mantri;
		while (current_mantri != NULL) {
			next_mantri = current_mantri->next_mantri_soldier;
			delete current_mantri;
			current_mantri = next_mantri;
		}

		Eliminated *current_soldier = this->first_soldier;
		Eliminated *next_soldier;
		while (current_soldier != NULL) {
			next_soldier = current_soldier->next_mantri_soldier;
			delete current_soldier;
			current_soldier = next_soldier;
		}
	}
};


class Player {
private:
	string colour;
	const Mantri* p_mantris;
	const Soldier* p_soldiers;
	BoardGame* board;
	EliminatedContainer e_container;
public:
	Player(const string & colour, const Mantri* mantris, const Soldier* soldiers, BoardGame & current_game) {
		this->colour = colour;
		p_mantris = mantris;
		p_soldiers = soldiers;
		board = &current_game;
		e_container = EliminatedContainer();
	}

	const string & get_colour() {
		return this->colour;
	}

	bool mantri_retrieval() {
		int col_ls = 0, row_ls, row_i, col_i;
		if (tolower(this->colour[0]) == 'w') {
			for (col_ls = 0, row_ls = 0; col_ls < this->board->get_size(); col_ls++)
				if (this->board->is_soldier(Location(row_ls, col_ls)) && this->board->is_black(Location(row_ls, col_ls))) break;
			if (col_ls == board->get_size()) return false;
			row_i = 6;
		}
		else if (tolower(this->colour[0]) == 'b') {
			for (col_ls = 0, row_ls = 6; col_ls < this->board->get_size(); col_ls++)
				if (this->board->is_soldier(Location(row_ls, col_ls)) && this->board->is_white(Location(row_ls, col_ls))) break;
			if (col_ls == board->get_size()) return false;
			row_i = 0;
		}

		Pawn* last_soldier = this->board->get_pawn(Location(row_ls, col_ls));

		for (col_ls = 0; col_ls < this->board->get_size(); col_ls++)
			if (this->board->is_empty(Location(row_i, col_ls))) break;
		if (col_ls == this->board->get_size()) return false;

		Pawn* retrieved_mantri = &this->board->clear;
		if (not this->e_container.retrieve_mantri(retrieved_mantri)) return false;

		INVALID:
		system("clear");
		if (tolower(this->colour[0]) == 'w') {
			this->board->display_black_board();
			cout << "\n\n\t\t\t\tBLACK";
		}
		else if(tolower(this->colour[0]) == 'b') {
			this->board->display_white_board();
			cout << "\n\n\t\t\t\tWHITE";
		}

		cout << "\n\t\t\tRetrieve Your Mantri!";
		cout << "\n\t\tChoose a Location: ";
		cout << "\n\t\t\tRow Index: ";
		cout << row_i + 1;
		cout << "\n\t\t\tColumn Index: ";
		cin >> col_i;
		col_i -= 1;

		if (col_i < 0 || col_i > 6) {
			cerr << "\n\t\t  Invalid Column!\n\t\t  ";
			system("pause");
			goto INVALID;
		}

		if (not (this->board->is_empty(Location(row_i, col_i)))) {
			cerr << "\n\t\t  Occupied Position!\n\t\t  ";
			system("pause");
			goto INVALID;
		}

		last_soldier->change_eliminated(true);
		last_soldier->change_location(Location(-1, -1));

		retrieved_mantri->change_eliminated(false);
		this->board->make_move(retrieved_mantri, Location(row_i, col_i));

		return true;
	}

	bool validate_move(char pawn_type, int pawn_num, int rowi, int coli, char& injury_type, int& injury_num) {
		bool injury_flag = false;
		if (tolower(pawn_type) != tolower(this->colour[0]))
			throw "Invalid Selection!";
		if (rowi < 0 || rowi > this->board->get_size() || coli < 0 || coli > this->board->get_size())
			throw "Invalid Position!";
		if (not (this->board->is_empty(Location(rowi, coli))))
			throw "Occupied Position!";

		Pawn & current_pawn = this->board->parse_pawn(pawn_type, pawn_num);

		if (not (current_pawn.is_valid_move(Location(rowi, coli))))
			throw "Invalid Move!";

		if (rowi == current_pawn.get_location().row_index + 2 || rowi == current_pawn.get_location().row_index - 2) {
			if ((current_pawn.is_white() && this->board->is_white(Location(rowi - 1, coli))) ||
					(current_pawn.is_black() && this->board->is_black(Location(rowi + 1, coli))))
				throw "Own Pawn Cannot be Eliminated!";
			if((current_pawn.is_white() && this->board->is_empty(Location(rowi - 1, coli))) ||
					(current_pawn.is_black() && this->board->is_empty(Location(rowi + 1, coli))))
				throw "Invalid Move!";

			int through_row_index, through_column_index;
			if (current_pawn.is_white())
				through_row_index = rowi - 1;
			else if (current_pawn.is_black())
				through_row_index = rowi + 1;
			cout << "\t\tChoose a Through Location: ";
			cout << "\n\t\t\tRow Index: ";
			cout << through_row_index + 1 << endl;
			cout << "\t\t\tColumn Index: ";
			cin >> through_column_index;
			through_column_index -= 1;

			if (not(coli == through_column_index + 1 || coli == through_column_index - 1) ||
					through_column_index < 0 || through_column_index > 6)
				throw "Invalid Through Location!";

			if (not(this->board->is_empty(Location(through_row_index, through_column_index)))) {
				if ((current_pawn.is_white() && this->board->is_white(Location(through_row_index, through_column_index))) ||
					(current_pawn.is_black() && this->board->is_black(Location(through_row_index, through_column_index))))
					throw "Own Pawn Cannot be Injured!";

				if (this->board->get_pawn(Location(through_row_index, through_column_index))->is_mantri())
					throw "Mantri Pawn Cannot be Injured!";
			}

			if (current_pawn.is_white()) {
				this->e_container.add_to_container(this->board->get_pawn(Location(rowi - 1, coli)));
				this->board->get_pawn(Location(rowi - 1, coli))->change_eliminated(true);
				this->board->make_move(this->board->get_pawn(Location(rowi - 1, coli)), Location(-1, -1));

				if (not this->board->is_empty(Location(through_row_index, through_column_index))) {
					injury_flag = true;
					injury_type = 'b';
					injury_num = this->board->get_pawn(Location(through_row_index, through_column_index))->get_index() + 1;
				}
			}
			else if (current_pawn.is_black()) {
				this->e_container.add_to_container(this->board->get_pawn(Location(rowi + 1, coli)));
				this->board->get_pawn(Location(rowi + 1, coli))->change_eliminated(true);
				this->board->make_move(this->board->get_pawn(Location(rowi + 1, coli)), Location(-1, -1));

				if (not this->board->is_empty(Location(through_row_index, through_column_index))) {
					injury_flag = true;
					injury_type = 'w';
					injury_num = this->board->get_pawn(Location(through_row_index, through_column_index))->get_index() + 1;
				}
			}

		}

		else {
			if (coli == current_pawn.get_location().column_index + 2) {
				if(this->board->is_empty(Location(rowi, coli - 1)))
					throw "Invalid Move!";

				if (current_pawn.is_white()) {
					if (rowi + 1 > 6)
						throw "Invalid Through Location!";
					if (this->board->is_white(Location(rowi, coli - 1)))
						throw "Own Pawn Cannot be Eliminated!";
					if (this->board->is_white(Location(rowi + 1, coli - 1)))
						throw "Own Pawn Cannot be Injured!";
					if (this->board->is_mantri(Location(rowi + 1, coli - 1)))
						throw "Mantri Pawn Cannot be Injured!";
					if (not this->board->is_empty(Location(rowi + 1, coli - 1))) {
						injury_flag = true;
						injury_type = 'b';
						injury_num = this->board->get_pawn(Location(rowi + 1, coli - 1))->get_index() + 1;
					}
				}

				else if (current_pawn.is_black()) {
					if (rowi - 1 < 0)
						throw "Invalid Through Location!";
					if (this->board->is_black(Location(rowi, coli - 1)))
						throw "Own Pawn Cannot be Eliminated!";
					if (this->board->is_black(Location(rowi - 1, coli - 1)))
						throw "Own Pawn Cannot be Injured!";
					if (this->board->is_mantri(Location(rowi - 1, coli - 1)))
						throw "Mantri Pawn Cannot be Injured!";
					if (not this->board->is_empty(Location(rowi - 1, coli - 1))) {
						injury_flag = true;
						injury_type = 'w';
						injury_num = this->board->get_pawn(Location(rowi - 1, coli - 1))->get_index() + 1;
					}
				}

				this->e_container.add_to_container(this->board->get_pawn(Location(rowi, coli - 1)));
				this->board->get_pawn(Location(rowi, coli - 1))->change_eliminated(true);
				this->board->make_move(this->board->get_pawn(Location(rowi, coli - 1)), Location(-1, -1));
			}

			else if (coli == current_pawn.get_location().column_index - 2) {
				if(this->board->is_empty(Location(rowi, coli + 1)))
					throw "Invalid Move!";

				if (current_pawn.is_white()) {
					if (rowi + 1 > 6)
						throw "Invalid Through Location!";
					if (this->board->is_white(Location(rowi, coli + 1)))
						throw "Own Pawn Cannot be Eliminated!";
					if (this->board->is_white(Location(rowi + 1, coli + 1)))
						throw "Own Pawn Cannot be Injured!";
					if (this->board->is_mantri(Location(rowi + 1, coli + 1)))
						throw "Mantri Pawn Cannot be Injured!";
					if (not this->board->is_empty(Location(rowi + 1, coli + 1))) {
						injury_flag = true;
						injury_type = 'b';
						injury_num = this->board->get_pawn(Location(rowi + 1, coli + 1))->get_index() + 1;
					}
				}

				else if (current_pawn.is_black()) {
					if (rowi - 1 < 0)
						throw "Invalid Through Location!";
					if (this->board->is_black(Location(rowi, coli + 1)))
						throw "Own Pawn Cannot be Eliminated!";
					if (this->board->is_black(Location(rowi - 1, coli + 1)))
						throw "Own Pawn Cannot be Injured!";
					if (this->board->is_mantri(Location(rowi - 1, coli + 1)))
						throw "Mantri Pawn Cannot be Injured!";
					if (not this->board->is_empty(Location(rowi - 1, coli + 1))) {
						injury_flag = true;
						injury_type = 'w';
						injury_num = this->board->get_pawn(Location(rowi - 1, coli + 1))->get_index() + 1;
					}
				}

				this->e_container.add_to_container(this->board->get_pawn(Location(rowi, coli + 1)));
				this->board->get_pawn(Location(rowi, coli + 1))->change_eliminated(true);
				this->board->make_move(this->board->get_pawn(Location(rowi, coli + 1)), Location(-1, -1));
			}
		}

		this->board->make_move(&current_pawn, Location(rowi, coli));
		return injury_flag;
	}

	bool validate_injury_move(char pawn_type, int pawn_num, int rowi, int coli) {
		if (tolower(pawn_type) != tolower(this->colour[0]))
			throw "Invalid Selection!";
		if (rowi < 0 || rowi > this->board->get_size() || coli < 0 || coli > this->board->get_size())
			throw "Invalid Position!";
		if (not (this->board->is_empty(Location(rowi, coli))))
			throw "Occupied Position!";

		Pawn & current_pawn = this->board->parse_pawn(pawn_type, pawn_num);
		if (not ((current_pawn.is_white() && (rowi == 1 || rowi == 2)) || (current_pawn.is_black() && (rowi == 4 || rowi == 5))))
			throw "Invalid Move!";

		this->board->make_move(&current_pawn, Location(rowi, coli));
		return true;
	}
};

int main() {
	BoardGame game_one;
	Player player_white("WHITE", game_one.get_white_mantris(), game_one.get_white_soldiers(), game_one);
	Player player_black("BLACK", game_one.get_black_mantris(), game_one.get_black_soldiers(), game_one);

	char char_pawn;
	enum player_turn {white, black} turn;
	int int_pawn, r_index, c_index;
	bool injury_flag = false;

	turn = white;

	while (not game_one.game_over()) {
		system("clear");

		if (turn == white) {
			game_one.display_white_board();
			cout << "\n\n\t\t\t\tWHITE";
		}
		else if (turn == black) {
			game_one.display_black_board();
			cout << "\n\n\t\t\t\tBLACK";
		}

		if (not injury_flag) {
			cout << "\n\t\tSelect pawn to be moved: ";
			cin >> char_pawn >> int_pawn;
		}
		else
			cout << "\n\t\t\tSoldier Pawn " << char_pawn << int_pawn << " injured! \n";

		cout << "\t\tChoose a Location: ";
		cout << "\n\t\t\tRow Index: ";
		cin >> r_index;
		cout << "\t\t\tColumn Index: ";
		cin >> c_index;

		r_index -= 1;
		c_index -= 1;

		try {
			if (not injury_flag) {
				if (turn == white) {
					injury_flag = player_white.validate_move(char_pawn, int_pawn, r_index, c_index, char_pawn, int_pawn);
					if (player_black.mantri_retrieval()) cout << "\n\t\t\tMantri Retrieved!";
					turn = black;
				}
				else if (turn == black) {
					injury_flag = player_black.validate_move(char_pawn, int_pawn, r_index, c_index, char_pawn, int_pawn);
					if (not player_white.mantri_retrieval()) cout << "\n\t\t\tMantri Retrieved!";
					turn = white;
				}
			}
			else {
				if (turn == white && player_white.validate_injury_move(char_pawn, int_pawn, r_index, c_index)) {
					injury_flag = false;
					turn = black;
				}
				else if (turn == black && player_black.validate_injury_move(char_pawn, int_pawn, r_index, c_index)) {
					injury_flag = false;
					turn = white;
				}
			}
		}
		catch (const char* msg) {
			cerr << "\n\t\t  " << msg << "\n\t\t  ";
			system("pause");
		}
	}

	if (not (turn == white)) {
		game_one.display_white_board();
		if (injury_flag) {
			cout << "\n\t\tSoldier Pawn " << char_pawn << int_pawn << " injured! \n";
			injury_flag = false;
		}
		cout << "\n\n\t\t\t **** WHITE WON! **** \n\n";
	}

	else if (not (turn == black)) {
		game_one.display_black_board();
		if (injury_flag) {
			cout << "\n\t\tSoldier Pawn " << char_pawn << int_pawn << " injured! \n";
			injury_flag = false;
		}
		cout << "\n\n\t\t\t **** BLACK WON! **** \n\n";
	}

	system("pause");
	return 0;
}
