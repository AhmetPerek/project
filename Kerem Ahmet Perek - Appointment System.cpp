#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[50], surname[50];
    char date[50];
    char appointment_time[50];
} customer;

static int nextID = 1;

int isValidDate(const char *date) {
    int day, month, year;

    if (sscanf(date, "%2d.%2d.%4d", &day, &month, &year) != 3) {
        return 0;
    }

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2023) {
        return 0;
    }

    return 1;
}

int isValidTime(const char *time) {
    int hour, minute;

    if (sscanf(time, "%d.%d", &hour, &minute) != 2 || (hour < 10 || hour > 17) || (minute != 0 && minute != 30)) {
        return 0;
    }

    return 1;
}

int isAppointmentAvailable(customer* appointments, const char *date, const char *time) {
    for (int i = 0; i < nextID - 1; i++) {
        if (strcmp(appointments[i].date, date) == 0 && strcmp(appointments[i].appointment_time, time) == 0) {
            return 0;
        }
    }

    return 1;
}

void getValidInput(const char *message, char *input, int (*validator)(const char *)) {
    int isValid;
    do {
        printf("%s", message);
        scanf("%s", input);

        isValid = validator(input);

        if (!isValid) {
            printf("Invalid input. ");
            if (validator == isValidDate) {
                printf("Please enter a date after 2023.\n");
            } else {
                printf("Please enter a valid value.\n");
            }
        } else {
            break;
        }

    } while (1);
}

void listAppointments(customer* appointments) {
    for (int i = 0; i < nextID - 1; i++) {
        printf("ID: %d, Name: %s %s, Date: %s, Time: %s\n",
               appointments[i].id,
               appointments[i].name, appointments[i].surname,
               appointments[i].date, appointments[i].appointment_time);
    }

    if (nextID == 1) {
        printf("No appointments have been taken.\n");
    }
}

void writeAppointmentsToFile(customer* appointments) {
    FILE *file = fopen("appointments.txt", "w");

    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < nextID - 1; i++) {
        fprintf(file, "ID: %d, Name: %s %s, Date: %s, Time: %s\n",
                appointments[i].id,
                appointments[i].name, appointments[i].surname,
                appointments[i].date, appointments[i].appointment_time);
    }

    fclose(file);

}

void bolum1(customer* appointments) {
    printf("\nYou can make an appointment from this section.\n");

    customer newapo;
    newapo.id = nextID++;

    printf("Please enter the name and surname: ");
    scanf("%s%s", newapo.name, newapo.surname);

    do {
        getValidInput("Please enter the appointment date (DD.MM.YYYY): ", newapo.date, isValidDate);

        getValidInput("Please enter the appointment time (HH.mm, between 10.00 and 18.00, in half-hour intervals): ", newapo.appointment_time, isValidTime);

        if (!isAppointmentAvailable(appointments, newapo.date, newapo.appointment_time)) {
            printf("Appointment not available at the specified date and time. Please choose another date and time.\n");
        } else {
            printf("An appointment has been made! Customer ID: %d\n", newapo.id);
            appointments[nextID - 2] = newapo;
            break;
        }
    } while (1);
}

void bolum2(customer* appointments) {
  printf("\nYou can update your appointment from this section.\n");

  int updateID;
  printf("Enter the ID of the appointment to update: ");
  scanf("%d", &updateID);

  int foundIndex = -1;
  for (int i = 0; i < nextID - 1; i++) {
    if (appointments[i].id == updateID) {
      foundIndex = i;
      break;
    }
  }

  if (foundIndex == -1) {
    printf("Appointment not found with the given ID.\n");
    return;
  }

  printf("Current Appointment Information:\n");
  printf("ID: %d, Name: %s %s, Date: %s, Time: %s\n",
      appointments[foundIndex].id,
      appointments[foundIndex].name, appointments[foundIndex].surname,
      appointments[foundIndex].date, appointments[foundIndex].appointment_time);

  char newDate[50];
  char newTime[50];

  do {
    getValidInput("Enter the new appointment date (DD.MM.YYYY): ", newDate, isValidDate);

    getValidInput("Enter the new appointment time (HH.mm, between 10.00 and 18.00, in half-hour intervals): ", newTime, isValidTime);

    if (!isAppointmentAvailable(appointments, newDate, newTime)) {
      printf("Appointment not available at the specified date and time. Please choose another date and time.\n");
    } else {
      break;
    }
  } while (1);

  printf("Updated Appointment Information:\n");
  printf("ID: %d, Name: %s %s, Date: %s, Time: %s\n",
      appointments[foundIndex].id,
      appointments[foundIndex].name, appointments[foundIndex].surname,
      newDate, newTime);

  char confirmation;
  printf("Do you want to update the appointment? (y/n): ");
  scanf(" %c", &confirmation);

  if (confirmation == 'y' || confirmation == 'Y') {
    strcpy(appointments[foundIndex].date, newDate);
    strcpy(appointments[foundIndex].appointment_time, newTime);
    printf("Appointment updated successfully!\n");
  } else {
    printf("Appointment update canceled.\n");
  }
}

void bolum3(customer* appointments) {
    printf("\nYou can cancel your appointment from this section.\n");

    int cancelID;
    printf("Enter the ID of the appointment to cancel: ");
    scanf("%d", &cancelID);

    int foundIndex = -1;
    for (int i = 0; i < nextID - 1; i++) {
        if (appointments[i].id == cancelID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Appointment not found with the given ID.\n");
        return;
    }

    printf("Appointment Information:\n");
    printf("ID: %d, Name: %s %s, Date: %s, Time: %s\n",
        appointments[foundIndex].id,
        appointments[foundIndex].name, appointments[foundIndex].surname,
        appointments[foundIndex].date, appointments[foundIndex].appointment_time);

    char confirmation;
    printf("Do you want to cancel the appointment? (y/n): ");
    scanf(" %c", &confirmation);

    if (confirmation == 'y' || confirmation == 'Y') {
        for (int i = foundIndex; i < nextID - 1; i++) {
            appointments[i] = appointments[i + 1];
            appointments[i].id--; 
        }

        nextID--; 

        printf("Appointment canceled successfully!\n");
    } else {
        printf("Appointment cancellation canceled.\n");
    }
}

void bolum4(customer* appointments) {
    printf("\nYou can see previously made appointments from this section.\n");
    listAppointments(appointments);
}

void bolum5(customer* appointments) {
	printf("\nTXT file is being updated!\n");
	writeAppointmentsToFile(appointments);
}

int main() {
    int choice;
    customer appointments[50];

    FILE *file = fopen("appointments.txt", "r");
	if (file != NULL) {
    int i = 0;
    while (fscanf(file, "ID: %d, Name: %49[^,], Date: %49[^,], Time: %49[^\n]\n",
                  &appointments[i].id,
                  appointments[i].name,
                  appointments[i].date,
                  appointments[i].appointment_time) == 4) {
        i++;
        if (i >= 50) {
            break;
        }
    }
    fclose(file);
    nextID = i + 1;
	}

    do {
        printf("\nMain Menu\n");
        printf("1. New Appointment\n");
        printf("2. Update Appointment\n");
        printf("3. Cancel Appointment\n");
        printf("4. Taken Appointments\n");
        printf("5. TXT Update\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                bolum1(appointments);
                break;
            case 2:
                bolum2(appointments);
                break;
            case 3:
                bolum3(appointments);
                break;
            case 4:
                bolum4(appointments);
                break;
            case 5:
                bolum5(appointments);
                break;
            case 0:
                printf("\nExiting the program.\n");
                break;
            default:
                printf("\nInvalid selection. Try again.\n");
        }

    } while (choice != 0);

    return 0;
}

