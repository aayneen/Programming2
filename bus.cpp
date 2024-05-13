#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

using namespace std;

struct BusTrip {
    string destination;
    string departureTime;
    double ticketPrice;
    int availableSeats;
};

struct BusTicket {
    string passengerName;
    int seatNumber;
    double ticketPrice;
    BusTrip trip;
};

vector<BusTrip> readBusTripsFromFile(const string& filename) {
    vector<BusTrip> trips;
    ifstream inputFile(filename);

    if (inputFile.is_open()) {
        BusTrip trip;
        while (inputFile >> trip.destination >> trip.departureTime >> trip.ticketPrice >> trip.availableSeats) {
            trips.push_back(trip);
        }
        inputFile.close();
    } else {
        cerr << "Error: Unable to open the file for reading.\n";
    }

    return trips;
}

void displayAvailableTrips(const vector<BusTrip>& trips) {
    cout << "\nAvailable Bus Trips From Putrajaya Sentral:\n";
    cout << "-----------------------------------------------------------------------\n";
    cout << "ID\tDestination\tDepartureTime\tPrice\t\tAvailableSeats\n";
    for (size_t i = 0; i < trips.size(); ++i) {
        cout << i + 1 << "\t" << trips[i].destination << "\t" << trips[i].departureTime
             << "\t\tRM" << fixed << setprecision(2) << trips[i].ticketPrice << "\t\t" << trips[i].availableSeats << "\n";
    }
    cout << "-----------------------------------------------------------------------\n";
}

void bookTicket(BusTicket& ticket, vector<BusTrip>& trips, vector<BusTicket>& bookedTickets) {
    int tripId;

    displayAvailableTrips(trips);

    cout << "\nEnter the ID of the trip you want to book: ";
    cin >> tripId;

    if (tripId >= 1 && tripId <= static_cast<int>(trips.size())) {
        ticket.trip = trips[tripId - 1];

        cout << "Enter passenger name: ";
        cin.ignore();
        getline(cin, ticket.passengerName);

        cout << "Enter seat number: ";
        cin >> ticket.seatNumber;

        if (ticket.seatNumber < 1 || ticket.seatNumber > ticket.trip.availableSeats) {
            cerr << "Error: Invalid seat number.\n";
            return;
        }

        for (const auto& bookedTicket : bookedTickets) {
            if (bookedTicket.trip.destination == ticket.trip.destination &&
                bookedTicket.trip.departureTime == ticket.trip.departureTime &&
                bookedTicket.seatNumber == ticket.seatNumber) {
                cerr << "\nError: Seat already booked for this trip.";
                cerr << "\nPlease book another ticket.\n";
                return;
            }
        }

        ticket.ticketPrice = ticket.trip.ticketPrice;

        trips[tripId - 1].availableSeats--;

        bookedTickets.push_back(ticket);
        cout << "\nTicket Booking Successful!\n";

    } else {
        cerr << "Error: Invalid trip ID.\n";
    }
}

void printReceipt(const BusTicket& ticket) {
    ofstream outputFile("receipt.txt", ios::app);
    if (outputFile.is_open()) {
        outputFile << "------------------- Receipt -------------------\n";
        outputFile << "Passenger Name: " << ticket.passengerName << endl;
        outputFile << "Destination: " << ticket.trip.destination << endl;
        outputFile << "Departure Time: " << ticket.trip.departureTime << endl;
        outputFile << "Seat Number: " << ticket.seatNumber << endl;
        outputFile << "Ticket Price: RM" << fixed << setprecision(2) << ticket.ticketPrice << endl;
        outputFile.close();
        cout << "\n\n";
        cout << "***********************************************************************" << endl;
        cout << "------------------- Receipt -------------------\n";
        cout << "Passenger Name: " << ticket.passengerName << endl;
        cout << "Destination: " << ticket.trip.destination << endl;
        cout << "Departure Time: " << ticket.trip.departureTime << endl;
        cout << "Seat Number: " << ticket.seatNumber << endl;
        cout << "Ticket Price: RM" << fixed << setprecision(2) << ticket.ticketPrice << endl;
        cout << "Receipt saved to 'receipt.txt'\n";
        cout << "***********************************************************************" << endl;
    } else {
        cerr << "Error: Unable to open the file for writing.\n";
    }
}

int main()
{
int choice;
vector<BusTrip> trips = readBusTripsFromFile("bus_trips.txt");
vector<BusTicket> bookedTickets;

while(true)
{

  cout<<"\n";

  cout<<"\t\t\t1.Display Available Buses\n\t\t\t"<<"2.Book a Ticket\n\t\t\t"<<"3.Show Receipt\n\t\t\t"<<"4.Exit";

  cout<<"\n\t\t\tEnter your choice:-> ";

  cin>>choice;

  switch (choice) {
            case 1: {
                displayAvailableTrips(trips);
                break;
            }
            case 2: {
                BusTicket ticket;
                bookTicket(ticket, trips, bookedTickets);
                break;
            }
            case 3: {
                for (const auto& ticket : bookedTickets) {
                    printReceipt(ticket);
                }
                break;
            }
            case 4:
                exit(0);
            default:
                cerr << "Error: Invalid choice. Please enter a valid option.\n";
                break;

  }

}

    return 0;
}
