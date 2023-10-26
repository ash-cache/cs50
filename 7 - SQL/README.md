# movies
This directory contains a `movies.db` SQLite database file that was adapted from IMDb [(link)](https://cdn.cs50.net/2021/fall/psets/7/movies.zip). The files named `#.sql` are SQL queries to answer some questions. 
```
movies/ $ sqlite3 movies.db
SQLite version 3.39.3 2022-09-05 11:02:23
Enter ".help" for usage hints.
sqlite> .schema
CREATE TABLE movies (
                    id INTEGER,
                    title TEXT NOT NULL,
                    year NUMERIC,
                    PRIMARY KEY(id)
                );
CREATE TABLE stars (
                movie_id INTEGER NOT NULL,
                person_id INTEGER NOT NULL,
                FOREIGN KEY(movie_id) REFERENCES movies(id),
                FOREIGN KEY(person_id) REFERENCES people(id)
            );
CREATE TABLE directors (
                movie_id INTEGER NOT NULL,
                person_id INTEGER NOT NULL,
                FOREIGN KEY(movie_id) REFERENCES movies(id),
                FOREIGN KEY(person_id) REFERENCES people(id)
            );
CREATE TABLE ratings (
                movie_id INTEGER NOT NULL,
                rating REAL NOT NULL,
                votes INTEGER NOT NULL,
                FOREIGN KEY(movie_id) REFERENCES movies(id)
            );
CREATE TABLE people (
                id INTEGER,
                name TEXT NOT NULL,
                birth NUMERIC,
                PRIMARY KEY(id)
            );
```

Here are some of the questions:

6. Determine the average rating of all movies released in 2012.

7. List all movies released in 2010 and their ratings, in descending order by rating. For movies with the same rating, order them alphabetically by title.

12. List the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.

13. List the names of all people who starred in a movie in which Kevin Bacon also starred.
  There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
  
# Fiftyville
This was a fun who-dun-it where the CS50 duck was stolen and I used SQL queries on a database of evidence to find the culprit.
