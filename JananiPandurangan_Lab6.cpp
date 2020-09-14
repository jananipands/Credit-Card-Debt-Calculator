/*
Janani Pandurangan
jananipandurangan05@gmail.com
Lab 6 - Credit Card Debt
*/

#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

void input(double& balanceOwed, int& aprPercentage, int& minPayPercentage);
void heading(ofstream& fileOutput, double balOwed, int apr, int minPay);
void calculate(double& balanceLeft, int aprRate, int minRate, double& interestPerMonth, double& interestAccumulated, double& minimumPerMonth, int& month, double& previousMonthInterest);
void output(ofstream& fileOutput, double& balanceRemaining, double& interestPaymentMonthly, double& totalInterest, double& minPayPerMonth, int monthCounter, int loopCount);

int main() 
{
	
	double balanceOwing, interestPM, interestTotal, minPM, prevMonthInterest;//included previous month interest for summing interest, did not declare in calculation since it would require initialization value
	int aprPercent, minPayPercent, monthCount, loopCounter; //aprPercent and minPayPercent have reference only in input(), not necessary to have references for it in other functions since its value doesn't change

	ofstream fileOutput; //passed as a reference to heading() and calculate() so that they can write to the file
	fileOutput.open("lab6_Output.txt"); 

	monthCount = 0; //this acts as a counter as it counts the amount of times the loop iterates and is outputed as the number of months 
	interestTotal = 0; //needed to be initialized here and passed on as a reference since initializing in calculate() would reset the value each iteration
	loopCounter = 0;

	input(balanceOwing, aprPercent, minPayPercent);
	 
	heading(fileOutput, balanceOwing, aprPercent, minPayPercent); //printed the initial information here instead of output() to prevent it being printed multiple times

	while (balanceOwing > 0) {
			calculate(balanceOwing, aprPercent, minPayPercent, interestPM, interestTotal, minPM, monthCount, prevMonthInterest);
			++loopCounter; //there is an if statement involving this in output() to prevent it from output the initial balance (624.00)
			output(fileOutput, balanceOwing, interestPM, interestTotal, minPM, monthCount,loopCounter);
	}

	fileOutput << "\nProgrammed By: Janani Pandurangan" << endl;
	fileOutput << "Email: jananipandurangan05@gmail.com" << endl;
	fileOutput << "Title: Lab 6 - Credit Card Debt" << endl;

	return 0; 

}

void input(double& balanceOwed, int& aprPercentage, int& minPayPercentage)
{
	/*Pre: balanceOwed - a reference for balanceOwing, aprPercentage - a reference for aprPercent, 
	                     minPayPercentage - a referenceminPayPercent
	  Post: nothing 
	  Purpose: to gather the value of the balance owed, APR, and the minimum payment percentage from the lab6.txt file 
	*/
	ifstream fileInput;

	fileInput.open("lab6.txt");

	if (!fileInput.is_open())
	{
		cout << "File not found!" << endl;
		exit(100);
	}
	else
	{
		fileInput >> balanceOwed;
		fileInput >> aprPercentage;
		fileInput >> minPayPercentage;
	}

}

void heading(ofstream& fileOutput, double balOwed, int apr, int minPay)
{
	/*Pre: fileOutput - a reference to fileOutput, balOwed - a reference to balanceOwing,
	       apr - a reference to aprPercent, minPay - a reference to minPayPercent
	  Post: nothing 
	  Purpose: to print out the initial values and the headers for the table 
	*/

    fileOutput << fixed << setprecision(2) << "Balance Owing: $ " << balOwed << endl;
    fileOutput << "APR as %  " << apr << endl;
	fileOutput << "Percent for minimum payment as %  " << minPay << endl;
	fileOutput << "\nMonth" << setw(18) << "Balance" << setw(20) << "Interest" << setw(15) << "Minimum" << setw(15) << "Sum of" << endl;
	fileOutput << setw(44) << "this Month" << setw(33) << "interest paid" << endl;

}

void calculate(double& balanceLeft, int aprRate, int minRate, double& interestPerMonth, double& interestAccumulated, double& minimumPerMonth, int& month, double& previousMonthInterest)
{
	/* Pre: balanceLeft - a reference for balanceOwing, aprRate - the apr rate, minRate - the minimum payment rate,
	        interestPerMonth - a reference for interestPM, interestAccumulated - interestTotal, minimumPerMonth - a reference for minPM,
			month - a reference for monthCount, previousMonthInterest - a reference for prevMonthInterest
	   Post: nothing
	   Purpose: to calculate the monthly interest payment, total interest, balance remaining, the minimum payment, and the month count
	*/
	double aprMonthlyRate = (static_cast<double> (aprRate) / 100.0 / 12.0);
	double minimumRate = (static_cast<double> (minRate) / 100.0);
	double principalAmount = minimumPerMonth - interestPerMonth;

	if (month == 0) // to make sure that the variables are not left unitialized the first iteration and don't cause errors because of that
	{
		previousMonthInterest = 0;
		interestAccumulated = 0;
	}

	interestPerMonth = balanceLeft * aprMonthlyRate;
	
	if (month == 1)
		interestAccumulated = previousMonthInterest + interestPerMonth;
	else 
		interestAccumulated = interestAccumulated + interestPerMonth;

	minimumPerMonth = (balanceLeft + interestPerMonth) * minimumRate;

	if ((minimumPerMonth < 15.00) && (balanceLeft - principalAmount > 0))
	{
		minimumPerMonth = 15.00;
	}

	balanceLeft = balanceLeft - principalAmount;

	if ((balanceLeft - principalAmount) < 0)
	{
		minimumPerMonth = balanceLeft + interestPerMonth;
		balanceLeft = 0.0;
	}	

	++month; 

}


void output(ofstream& fileOutput, double& balanceRemaining, double& interestPaymentMonthly, double& totalInterest, double& minPayPerMonth, int monthCounter, int loopCount)
{
	/* Pre: fileOutput - a reference for fileOutput, balanceRemaining - a reference for balanceOwing, interestPaymentMonthly - a reference for interestPM,
	        totalInterest - a reference for interestTotal, minPayPerMonth - a reference for minPM, monthCounter - a reference for monthCount, loopCount - to count the amount of time
			the function iterates
	   Post: nothing
	   Purpose: to print the month count, the remaining balance, the monthly interest payment, the monthly minimum payment, and the total interest payment
	*/

	if (loopCount > 1) //needed to avoid the first loop output since it printed the initial balance and the same interest amount
	{
		fileOutput << fixed << setprecision(2);
		fileOutput << setw(2) << monthCounter - 1 << setw(21) << balanceRemaining << setw(18) << interestPaymentMonthly << setw(16) << minPayPerMonth << setw(16) << totalInterest << endl;
	}

}
	