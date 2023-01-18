-- Examine crime scene reports from July 28, 2021 on Humphrey Street
SELECT * FROM crime_scene_reports WHERE day = 28 AND month = 7 AND year = 2021 AND street = 'Humphrey Street';
-- Report id 295 provides the pertinent information: Theft took place at 10:15am at the Humphrey Street bakery.
-- There were 3 witnesses and each of their transcripts mentions the bakery.

-- Query for interviews matching that day and containing the word 'bakery'
SELECT * FROM interviews WHERE day = 28 AND month = 7 AND year = 2021 AND transcript LIKE "%bakery%";
-- Lead 1: Witness Ruth stated that the thief entered a car in the bakery parking lot and drove away within 10 minutes of the theft.

-- Lead 2: Witness Eugene recognized the thief as someone who was withdrawing money from the ATM on Leggett Street earlier that morning

-- Lead 3: Witness Raymond stated that the thief made a call as they were leaving the bakery. The thief stated their intention to take the
-- earliest flight out of Fiftyville tomorrow and asked the person on the other end to purchase the flight ticket.

-- Following Lead 1: Examine the bakery_security_logs on 7/28/21 between 10:15 am and 10:25 am.
SELECT * FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND minute >= 15 AND minute <= 25;

-- There are 8 cars that left the parking lot at that time. The license plates recorded can be looked up in the people table
-- to get names of possible suspects.
SELECT * FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND year = 2021 AND hour = 10 AND minute >=15 AND minute <=25) ORDER BY name;
-- Possible suspects are Barry, Bruce, Diana, Iman, Kelsey, Luca, Sofia, Vanessa. These individuals had opportunity.

-- Following Lead 2: Query for withdrawal transactions from the ATM on Leggett Street on the day in question.
SELECT * FROM atm_transactions WHERE day = 28 AND month = 7 AND year = 2021 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
-- 8 account numbers made a withdrawal matching the description.

-- Query for the names associated with the identified account numbers.
SELECT name
  FROM people
 WHERE id IN
       (SELECT person_id
          FROM bank_accounts
         WHERE account_number IN
               (SELECT account_number
               FROM atm_transactions
               WHERE day = 28
               AND MONTH = 7
               AND YEAR = 2021
               AND atm_location = 'Leggett Street'
               AND transaction_type = 'withdraw'))
ORDER BY name;
-- 8 suspects were identified: Benista, Brooke, Bruce, Diana, Iman, Kenny, Luca, Taylor

-- We can intersect the suspects from the ATM lead and the suspects from the bakery security logs.
SELECT name
  FROM people
 WHERE license_plate IN
       (SELECT license_plate
          FROM bakery_security_logs
         WHERE day = 28
           AND month = 7
           AND year = 2021
           AND hour = 10
           AND minute >=15
           AND minute <=25)

INTERSECT

SELECT name
  FROM people
 WHERE id IN
       (SELECT person_id
       FROM bank_accounts
       WHERE account_number IN
             (SELECT account_number
                FROM atm_transactions
               WHERE day = 28
                 AND MONTH = 7
                 AND YEAR = 2021
                 AND atm_location = 'Leggett Street'
                 AND transaction_type = 'withdraw'))
ORDER BY name;

-- Bruce, Diana, Iman, and Luca were at both locations and remain possible suspects.

-- Lead 3: Identify calls made by Bruce, Diana, Iman, or Luca on 7/28/21 that lasted less than a minute.
SELECT * FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE name IN ('Bruce', 'Diana', 'Iman', 'Luca')) AND day = 28 AND month = 7 AND year = 2021 AND duration < 60;

-- There were 2 calls made fitting this description. Look up the callers in the people database.
SELECT name FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE caller IN (SELECT phone_number FROM people WHERE name IN ('Bruce', 'Diana', 'Iman', 'Luca')) AND day = 28 AND month = 7 AND year = 2021 AND duration < 60);
-- Diana and Bruce both fit the description.

-- Look through the airports table to obtain the airport id for Fiftyville. Use this to look for flights leaving
-- Fiftyville on 7/29/21, order by hour and then by minute.
SELECT * FROM flights WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville') AND day = 29 AND month = 7 AND year = 2021 ORDER BY hour, minute;
-- The earliest flight leaving Fiftyville on 7/29 has id 36, leaves at 8:20 for a destination with airport code 4.

-- Look up this airport in the airports table.
SELECT full_name, city FROM airports WHERE id = 4;
-- This is LaGuardia Airport in New York City.

-- Look up Diana and Bruce's passport numbers and compare them against the passenger manifest for flight with id 36.
SELECT * from passengers WHERE passport_number IN (SELECT passport_number FROM people WHERE name in ('Diana', 'Bruce')) AND flight_id = 36
-- The passenger in seat 4A with passport_number 5773159633 is either Diana or Bruce.

-- Look up the name of the owner of the passport in seat 4A
SELECT name FROM people WHERE passport_number = (SELECT passport_number FROM passengers WHERE flight_id = 36 AND seat = '4A');
-- Result: Bruce

-- Look up Bruce's phone number, then look up calls he made on 7/28 lasting less than a minute, then look up the name of the receiver.
SELECT name FROM people WHERE phone_number = (SELECT receiver FROM phone_calls WHERE caller = (SELECT phone_number FROM people WHERE name = 'Bruce') AND day = 28 AND month = 7 AND year = 2021 AND duration < 60);
-- The receiver was Robin

-- CONCLUSION: Bruce stole the CS50 Duck from the Humphrey Street Bakery on 7/28. His accomplice was Robin who purchased a flight for Bruce
-- to leave Fiftyville on 7/29 for New York City.
