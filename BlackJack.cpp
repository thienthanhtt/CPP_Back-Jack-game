#include <iostream>
#include <string>
#include <cstdlib>
#include <Windows.h>
#include <cmath>

#define MAX 10

using namespace std;

struct Card
{
	int suit;	//1 (Bich), 2 (Chuon), 3 (Ro), 4 (Co)
	int value;	// 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 (J), 12 (Q), 13 (K), 14 (A)
};

struct Player
{
	int n;
	Card* cards;
};

void input(Card& c)
{
	cout << "Hinh (1 = Bich, 2 = Chuon, 3 = Ro, 4 = Co): ";
	cin >> c.suit;
	while (c.suit <= 0 || c.suit > 4)
	{
		cout << "Loi! Nhap lai: ";
		cin >> c.suit;
	}
	cout << "Gia tri (rieng J = 11, Q = 12, K = 13, A = 14): ";
	cin >> c.value;
	while (c.value < 2 || c.value > 14)
	{
		cout << "Loi! Nhap lai: ";
		cin >> c.value;
	}
}

string getCard(Card c)
{
	string name;

	switch (c.value)
	{
	case 2:
		name = "2";
		break;
	case 3:
		name = "3";
		break;
	case 4:
		name = "4";
		break;
	case 5:
		name = "5";
		break;
	case 6:
		name = "6";
		break;
	case 7:
		name = "7";
		break;
	case 8:
		name = "8";
		break;
	case 9:
		name = "9";
		break;
	case 10:
		name = "10";
		break;
	case 11:
		name = "J";
		break;
	case 12:
		name = "Q";
		break;
	case 13:
		name = "K";
		break;
	case 14:
		name = "A";
		break;
	default:
		break;
	}

	switch (c.suit)
	{
	case 1:
		name += " Bich";
		break;
	case 2:
		name += " Chuon";
		break;
	case 3:
		name += " Ro";
		break;
	case 4:
		name += " Co";
		break;
	default:
		break;
	}

	return name;
}

void swap(Card& c1, Card& c2)
{
	Card temp = c1;
	c1 = c2;
	c2 = temp;
}

void initializeCard(Card*& deck)
{
	deck = new Card[52];
	int numCard = 51;

	for (int i = 2; i <= 14; i++)
		for (int j = 1; j <= 4; j++)
		{
			deck[numCard].value = i;
			deck[numCard].suit = j;
			numCard--;
		}
}

void shuffleDeck(Card*& deck)
{
	srand(time(NULL));
	for (int i = 0; i < 52; i++)
		swap(deck[i], deck[rand() % 52]);
}

void drawCard(Card*& deck, int& n, Card c)
{
	for (int i = 0; i < n; i++)
		if (deck[i].suit == c.suit && deck[i].value == c.value)
			for (int j = i; j < n; j++)
				deck[j] = deck[j + 1];
	n--;
}

void outputDeck(Card*& deck, int n)
{
	for (int i = 0; i < n; i++)
		if (i == 0)
			cout << getCard(deck[i]) << "\t";
		else if (i % 10 == 0)
			cout << getCard(deck[i]) << endl;
		else
			cout << getCard(deck[i]) << "\t";
}

int point(Card* player, int n)
{
	int total = 0, countAce = 0;
	 
	for (int i = 0; i < n; i++)
		if (player[i].value > 10 && player[i].value < 14)
			total += 10;
		else if (player[i].value == 14)
			countAce++;
		else
			total += player[i].value;

		while (countAce > 0)
		{
			if (total > 11)
				total += 1;
			else if (total < 11)
				total += 11;
			else
				total += 10;

			countAce--;
		}
		
	return total;
}

int status(Card* player, int n)	// 0: xi bang, 1: ngu linh, 2: xi dach, 3: du tuoi, 4: quac, 5: chua du tuoi
{
	if (n == 5)
		if (point(player, n) <= 21)
			return 1;
		else
			return 4;
	if (n == 2)
	{
		if (player[0].value == 14 && player[1].value == 14)
			return 0;
		else if (player[0].value == 14 || player[1].value == 14)
		{
			if (player[0].value + player[1].value >= 24)
				return 2;
		}
	}
	if (point(player, n) < 16)
		return 5;
	else if (point(player, n) > 21)
		return 4;
	else
		return 3;
}

string status_(Card* player, int n)
{
	int stt = status(player, n);
	switch (stt)
	{
	case 0:
		return "Xi bang";
		break;
	case 1:
		return "Ngu linh";
		break;
	case 2:
		return "Xi dach";
		break;
	case 3:
		return "Du tuoi";
		break;
	case 4:
		return "Quac";
		break;
	case 5:
		return "Chua du tuoi";
		break;
	default:
		break;
	}
	return " ";
}

int win(Card* player, int n, Card* host, int nh)	//-1: thua, 0: hoa, 1: thang
{
	if (status(player, n) == 5)
		return -1;
	else if (status(player, n) == 1 && status(host, nh) == 1)
		if (point(player, n) < point(host, nh))
			return 1;
		else
			return -1;
	else if (status(player, n) == status(host, nh))
		return 0;
	else if (status(player, n) < status(host, nh))
		return 1;
	else
		return -1;
}

int main()
{
	Card* deck = nullptr;
	int n = 52, select, p;
	char cont;
	Player* player;

	srand(time(NULL));

	initializeCard(deck);
	shuffleDeck(deck);

	cout << "Nhap so luong nguoi choi: ";
	input:	cin >> p;
	if (p <= 0 || p > MAX)
		goto input;

	player = new Player[p + 1];

	//---------NGUOI CHOI -------

	for (int i = 0; i < p; i++)
	{
		cout << "Nguoi choi thu " << i + 1 << ":\n";
		player[i].n = 2;
		player[i].cards = new Card[player[i].n];
		for (int j = 0; j < player[i].n; j++)
		{
			player[i].cards[j] = deck[n - 1];
			drawCard(deck, n, player[i].cards[j]);
			cout << "La thu " << j + 1 << ": " << getCard(player[i].cards[j]) << endl;
		}
		cout << "Sau khi nho bai cua minh vui long nhan phim bat ky (tru phim enter va space) de tiep tuc ";
		cin >> cont;

		system("cls");
	}

	for (int i = 0; i < p; i++)
	{
		cout << "Nguoi choi thu " << i + 1 << ":\n";
		do {
			cout << "Ban muon rut tiep hay dan (1: rut, 2: dan): ";
			cin >> select;
			while (select != 1 && select != 2)
			{
				cout << "Khong hop le! Nhap lai: ";
				cin >> select;
			}

			if (select == 1)
			{
				player[i].n++;
				player[i].cards[player[i].n - 1] = deck[n - 1];
				drawCard(deck, n, player[i].cards[player[i].n - 1]);
				cout << "+--------------------------------+\n";
				for (int j = 0; j < player[i].n; j++)
					cout << "\tLa thu " << j + 1 << ": " << getCard(player[i].cards[j]) << endl;
				cout << "+--------------------------------+\n";
				cout << "Diem hien tai: " << point(player[i].cards, player[i].n) << endl;
			}
		} while (select == 1);
		cout << "Sau khi nho bai cua minh vui long nhan phim bat ky (tru phim enter va space) de tiep tuc ";
		cin >> cont;

		system("cls");
	}

	//---------- NHA CAI -------
	cout << "Luot cua nha cai:\n";
	player[p].n = 2;
	player[p].cards = new Card[player[p].n];
	for (int j = 0; j < player[p].n; j++)
	{
		player[p].cards[j] = deck[n - 1];
		drawCard(deck, n, player[p].cards[j]);
		cout << "La thu " << j + 1 << ": " << getCard(player[p].cards[j]) << endl;
	}

	do {
		cout << "Ban muon rut tiep hay dan (1: rut, 2: dan): ";
		cin >> select;
		while (select != 1 && select != 2)
		{
			cout << "Khong hop le! Nhap lai: ";
			cin >> select;
		}

		if (select == 1)
		{
			player[p].n++;
			player[p].cards[player[p].n - 1] = deck[n - 1];
			drawCard(deck, n, player[p].cards[player[p].n - 1]);
			cout << "+--------------------------------+\n";
			for (int j = 0; j < player[p].n; j++)
				cout << "\tLa thu " << j + 1 << ": " << getCard(player[p].cards[j]) << endl;
			cout << "+--------------------------------+\n";
			cout << "Diem hien tai: " << point(player[p].cards, player[p].n) << endl;
		}
	} while (select == 1);

	//----------- KET QUA --------

	cout << "\t*** KET QUA ***\n";
	for (int i = 0; i < p; i++)
		cout << "Nguoi choi thu " << i + 1 << ": " << point(player[i].cards, player[i].n) << " diem => " << status_(player[i].cards, player[i].n) << endl;
	cout << "Nha cai: " << point(player[p].cards, player[p].n) << " diem => " << status_(player[p].cards, player[p].n) << endl;

	cout << endl;
	for (int i = 0; i < p; i++)
		if (win(player[i].cards, player[i].n, player[p].cards, player[p].n) == -1)
			cout << "Nguoi choi " << i + 1 << ": thua" << endl;
		else if (win(player[i].cards, player[i].n, player[p].cards, player[p].n) == -0)
			cout << "Nguoi choi " << i + 1 << ": hoa" << endl;
		else
			cout << "Nguoi choi " << i + 1 << ": thang" << endl;

	for (int i = 0; i <= p; i++)
		delete[] player[i].cards;
	delete[] player;
	delete[] deck;
}