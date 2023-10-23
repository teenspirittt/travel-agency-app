CREATE TABLE "Employees" (
  "id" serial PRIMARY KEY,
  "full_name" varchar(255) NOT NULL,
  "address" text,
  "date_of_birth" date,
  "position" varchar(255),
  "salary" "decimal(10, 2)"
);

CREATE TABLE "EmployeeTransfers" (
  "id" serial PRIMARY KEY,
  "employee_id" int NOT NULL,
  "new_position" varchar(255),
  "transfer_reason" text,
  "order_number" varchar(50),
  "order_date" date
);

CREATE TABLE "Hotels" (
  "id" serial PRIMARY KEY,
  "name" varchar(255) NOT NULL,
  "class" int,
  "room_category" varchar(255)
);

CREATE TABLE "Flights" (
  "id" serial PRIMARY KEY,
  "flight_number" varchar(50) NOT NULL,
  "departure_date" timestamp NOT NULL,
  "aircraft_id" int,
  "class" varchar(50),
  "available_seats" int,
  "carrier_id" int
);

CREATE TABLE "Route" (
  "id" serial PRIMARY KEY,
  "name" varchar(255) NOT NULL,
  "country" varchar(255),
  "city" varchar(255),
  "duration" int,
  "hotel_id" int,
  "flight_id" int,
  "employee_id" int,
  "agency_representative_name" varchar(255),
  "agency_representative_phone" varchar(20)
);

CREATE TABLE "Clients" (
  "id" serial PRIMARY KEY,
  "full_name" varchar(255) NOT NULL,
  "phone" varchar(20) NOT NULL,
  "order_date" timestamp NOT NULL,
  "route_id" int,
  "class" varchar(50),
  "seat" varchar(10)
);

CREATE TABLE "Carriers" (
  "id" serial PRIMARY KEY,
  "name" varchar(255) NOT NULL
);

CREATE TABLE "Aircraft" (
  "id" serial PRIMARY KEY,
  "aircraft_type" varchar(255) NOT NULL,
  "carrier_id" int,
  "manufacturer" varchar(255),
  "capacity" int
);

ALTER TABLE "EmployeeTransfers" ADD FOREIGN KEY ("employee_id") REFERENCES "Employees" ("id");

ALTER TABLE "Flights" ADD FOREIGN KEY ("aircraft_id") REFERENCES "Aircraft" ("id");

ALTER TABLE "Route" ADD FOREIGN KEY ("hotel_id") REFERENCES "Hotels" ("id");

ALTER TABLE "Route" ADD FOREIGN KEY ("flight_id") REFERENCES "Flights" ("id");

ALTER TABLE "Route" ADD FOREIGN KEY ("employee_id") REFERENCES "Employees" ("id");

ALTER TABLE "Clients" ADD FOREIGN KEY ("route_id") REFERENCES "Route" ("id");

ALTER TABLE "Aircraft" ADD FOREIGN KEY ("carrier_id") REFERENCES "Carriers" ("id");
