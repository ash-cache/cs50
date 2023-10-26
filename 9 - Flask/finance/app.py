import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]

    # count = db.execute("SELECT count(owner) FROM ledger WHERE owner = ?", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    stocks = db.execute("SELECT symbol, shares FROM ledger WHERE owner = ?", user_id)
    cash_usd = usd(cash[0]["cash"])

    for row in stocks:
        details = lookup(row["symbol"])
        row["name"] = details["name"]
        row["price"] = usd(details["price"])
        row["total"] = usd(int(row["shares"]) * float(details["price"]))

    grand_total = 0
    for row in stocks:
        # Total is a string. Truncate the first char which is a '$'.
        total = row["total"][1:]
        # Remove commas from numbers
        total = total.replace(',', '')
        grand_total += float(total)

        # Do the same for Cash
        cash_total = float(cash_usd[1:].replace(',',''))

    grand_total += cash_total

    # Convert back to USD
    grand_total = usd(grand_total)

    return render_template("portfolio.html", cash=cash_usd, stocks=stocks, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "GET":
        return render_template("buy.html")

    elif request.method == "POST":
        # Canonicalize symbol for storing in db
        symbol = request.form.get("symbol").upper()

        # lookup returns a dict, get the value for the "price" key
        price = lookup(symbol)["price"]

        # Quantity is returned as a str and needs to be converted to int
        quantity = int(request.form.get("quantity"))

        # Get the total cost of the purchase
        purchase = price * quantity

        user_id = session["user_id"]

        # The SQL query returns a list with a dict inside: e.g. [{"cash": 10000}].
        # We need to index into the 0th row and get the value for the key of "cash".
        # Then we need to cast that to a float.
        row = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        balance = float(row[0]["cash"])

        if not price:
            return apology("Invalid stock")
        elif quantity < 0:
            return apology("Quanty must be a positive integer")
        elif purchase > balance:
            return apology("Not enough cash left")
        else:
            # First update the cash balance for the user
            db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", purchase, user_id)

            # Then add a record of the purchase to the history table
            db.execute("INSERT INTO history (owner, transact, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", user_id, "BUY", symbol, quantity, price)

            # Create a record of the user's ownership of a stock only if it does not already exist. Set it to 0.
            db.execute("INSERT OR IGNORE INTO ledger (owner, symbol, shares) VALUES (?, ?, ?)", user_id, symbol, 0)

            # Then update the number of shares held
            db.execute("UPDATE ledger SET shares = shares + ? WHERE owner = ? AND symbol = ?", quantity, user_id, symbol)

            # Redirect user to home page
            return redirect("/")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history")

    return render_template("history.html", history=history)




@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id, but if there were flashed messages, make them available in the next session
    if session.get("_flashes"):
        flashes = session.get("_flashes")
        session.clear()
        session["_flashes"] = flashes
    else:
        session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # return apology("TODO")
    if request.method == "GET":
        return render_template("quote.html")

    elif request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol)
        return render_template("quoted.html", name=quote["name"], price=quote["price"], symbol=quote["symbol"])



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    ## Register the user
    if request.method == "POST":

        uname = request.form.get("username")
        pw = request.form.get("password")

        pwhash = generate_password_hash(pw)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", uname, pwhash)
        flash("Account was registered. You may now log in.")
        return redirect("/login")


    # User reached route via GET (as by clicking a link or via redirect)
    ## Display registration form
    elif request.method == "GET":
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":
        user_id = session["user_id"]
        # Get stocks and quantity owned by the user
        stocks = db.execute("SELECT symbol, shares FROM ledger WHERE owner = ?", user_id)

        return render_template("sell.html", stocks=stocks)

    elif request.method == "POST":

        # Canonicalize symbol
        symbol = request.form.get("symbol").upper()

        # lookup returns a dict, get the value for the "price" key
        price = lookup(symbol)["price"]

        # Quantity is returned as a str and needs to be converted to int
        quantity = int(request.form.get("shares"))

        # Get the total cost of the purchase
        purchase = price * quantity

        user_id = session["user_id"]

        if not price:
            return apology("Invalid stock")
        elif quantity < 0:
            return apology("Quantity must be a positive integer")
        elif quantity > int(db.execute("SELECT symbol, shares FROM ledger WHERE symbol = ? AND owner = ?", symbol, user_id)[0]["shares"]):
            return apology("You don't have that many shares")
        else:
            # First update the ledger
            db.execute("UPDATE ledger SET shares = shares - ? WHERE symbol = ? AND owner = ?", quantity, symbol, user_id)

            # Then add a record of the purchase to the history table
            db.execute("INSERT INTO history (owner, transact, symbol, shares, price) VALUES (?, ?, ?, ?, ?)", user_id, "SELL", symbol, quantity, price)

            # Then update the cash balance for the user
            db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", purchase, user_id)

            # Redirect user to home page
            return redirect("/")
