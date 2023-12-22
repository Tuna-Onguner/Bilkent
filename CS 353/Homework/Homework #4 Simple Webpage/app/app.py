# -*- coding: utf-8 -*-

# Fall 2023 CS353 Homework #4
# Deniz Tuna Onguner 22001788
# 8 November 2023 Wednesday

import os

import MySQLdb.cursors
from flask import Flask, render_template, request, redirect, url_for, session, flash
from flask_mysqldb import MySQL

app = Flask(__name__)

app.secret_key = 'your_secret_key'

app.config['MYSQL_HOST'] = 'db'
app.config['MYSQL_USER'] = 'root'
app.config['MYSQL_PASSWORD'] = 'password'
app.config['MYSQL_DB'] = 'cs353hw4db'

mysql = MySQL(app)


@app.route('/')
@app.route('/login', methods=['GET', 'POST'])
def login():
    message = None
    if request.method == 'POST' and 'username' in request.form and 'password' in request.form:
        sname = request.form['username']
        sid = request.form['password']
        
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM student WHERE sname = %s AND sid = %s', (sname, sid,))
        student = cursor.fetchone()

        if student:
            session['loggedin'] = True
            session['userid'] = student['sid']
            session['username'] = student['sname']

            return redirect(url_for('main'))

        else:
            message = 'Please enter correct email or password!'

    return render_template('login.html', message=message)


@app.route('/register', methods=['GET', 'POST'])
def register():
    message = None
    if request.method == 'POST' and 'username' in request.form and 'id' in request.form and 'birthdate'\
            in request.form and 'department' in request.form and 'year' in request.form and 'gpa' in request.form:
        sname = request.form['username']
        sid = request.form['id']
        bdate = request.form['birthdate']
        dept = request.form['department']
        year = request.form['year']
        gpa = request.form['gpa']

        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('SELECT * FROM student WHERE sname = %s', (sname,))
        student = cursor.fetchone()

        if student:
            message = 'Choose a different username!'
            
        elif not sname or not sid or not bdate or not dept or not year or not gpa:
            message = 'Please fill out the entire form!'
            
        elif not 0.0 < float(gpa) <= 4.0:
            message = 'GPA should be in between 0 and 4!'
            
        else:
            cursor.execute('INSERT INTO student VALUES (%s, %s, %s, %s, %s, %s)', (sid, sname, bdate, dept, year, gpa,))
            mysql.connection.commit()
            
            #message = 'Student created successfully!'
            #flash(message, 'success')

            return redirect(url_for('login'))
            
    elif request.method == 'POST':
        message = 'Please fill all the fields!'
        
    return render_template('register.html', message=message)


@app.route('/main', methods=['GET', 'POST'])
def main():
    if 'loggedin' in session and session['loggedin']:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute("""
            SELECT c.cid, c.cname, c.quota, c.gpa_threshold
            FROM company c
            JOIN apply a ON c.cid = a.cid
            WHERE a.sid = %s
        """, (session['userid'],))
        companies = cursor.fetchall()
        company_list = [dict(company) for company in companies]
            
        for company in company_list:
            company['cname'] = company['cname'].capitalize()
            
        return render_template('main.html', company_list=company_list)
        
    else:
        return redirect(url_for('login'))


@app.route('/cancel_application', methods=['POST'])
def cancel_application():
    if 'loggedin' in session and session['loggedin']:
        cid, sid = request.form['company_id'], session['userid']
        
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute('DELETE FROM apply WHERE sid = %s AND cid = %s', (sid, cid,))
        mysql.connection.commit()
        
        cursor.execute('UPDATE company SET quota = quota + 1 WHERE cid = %s', (cid,))
        mysql.connection.commit()
        
        message = "Application successfully canceled."
        flash(message, 'success')

        return redirect(url_for('main'))
        
    else:
        return redirect(url_for('login'))
        

companies_available, flag = None, True


@app.route('/apply_for_new_internship', methods=['GET'])
def apply_for_new_internship():
    if 'loggedin' in session and session['loggedin']:
        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        cursor.execute("""
            SELECT COUNT(*) AS application_count
            FROM apply
            WHERE sid = %s
        """, (session['userid'],))
        app_count_fetched = cursor.fetchone()
        app_count = app_count_fetched['application_count']
        
        global flag
        if flag:
            if app_count >= 3:
                message = 'Only three applications are allowed for each student, you cannot make more new applications!'
                flash(message, 'danger')
                
                return redirect(url_for('main'))
                
            else:
                cursor.execute('SELECT gpa FROM student WHERE sid = %s', sid := (session['userid'],))
                gpa_fetched = cursor.fetchone()
                gpa = gpa_fetched['gpa']
                
                cursor.execute("""
                    SELECT c.*
                    FROM company c
                    WHERE c.quota > 0
                    AND c.gpa_threshold <= %s
                    AND c.cid NOT IN (
                        SELECT a.cid
                        FROM apply a
                        WHERE a.sid = %s
                    )
                """, (gpa, sid,))
                companies = cursor.fetchall()
                company_list = [dict(company) for company in companies]
                
                for company in company_list:
                    company['cname'] = company['cname'].capitalize()
                    
                global companies_available
                companies_available = company_list

                return render_template('apply.html', company_list=company_list)
                
        else:
            flag = True
            return redirect(url_for('main'))


@app.route('/apply', methods=['POST'])
def apply():
    if 'loggedin' in session and session['loggedin'] and 'cid' in request.form:
        cid = request.form['cid']
        
        global companies_available
        for company in companies_available:
            if cid == company['cid']:
                cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
                cursor.execute('INSERT INTO apply VALUES (%s, %s)', (session['userid'], cid,))
                mysql.connection.commit()
                
                cursor.execute('UPDATE company SET quota = quota - 1 WHERE cid = %s', (cid,))
                mysql.connection.commit()
        
                global flag
                flag = False
                return redirect(url_for('apply_for_new_internship'))
        
        message = "Not eligable to apply to this company"
        flash(message, 'danger')
        
        return redirect(url_for('main'))
    
    else:
        return redirect(url_for('login'))
        
        
@app.route('/summary', methods=['GET'])
def summary():
    if 'loggedin' in session and session['loggedin']:
        sid = session['userid']

        cursor = mysql.connection.cursor(MySQLdb.cursors.DictCursor)
        
        cursor.execute("""
            SELECT c.cname, c.quota, c.gpa_threshold
            FROM company c
            JOIN apply a ON c.cid = a.cid
            WHERE a.sid = %s
            ORDER BY c.quota DESC
        """, (sid,))
        companies = cursor.fetchall()
        company_list = [dict(company) for company in companies]
        
        for company in company_list:
            company['cname'] = company['cname'].capitalize()
        
        cursor.execute("""
            SELECT MAX(c.gpa_threshold) AS max_gpa_threshold, MIN(c.gpa_threshold) AS min_gpa_threshold
            FROM company c
            JOIN apply a ON c.cid = a.cid
            WHERE a.sid = %s
            GROUP BY a.sid
        """, (sid,))
        min_and_max_gpat = cursor.fetchone()
        if min_and_max_gpat:
            min_gpat = round(min_and_max_gpat['min_gpa_threshold'], 2)
            max_gpat = round(min_and_max_gpat['max_gpa_threshold'], 2)
        else:
            min_gpat, max_gpat = None, None
        
        cursor.execute("""
            SELECT c.city, COUNT(*) AS application_count
            FROM company c
            JOIN apply a ON c.cid = a.cid
            WHERE a.sid = %s
            GROUP BY c.city
        """, (sid,))
        cities_and_app_counts_fetched = cursor.fetchall()
        cities_and_app_counts = [dict(city_and_count) for city_and_count in cities_and_app_counts_fetched]
        
        cursor.execute("""
            SELECT c.cname AS company_with_max_quota, d.cname AS company_with_min_quota
            FROM company c
            JOIN apply a ON c.cid = a.cid
            JOIN company d ON d.cid = a.cid
            WHERE a.sid = %s
            AND c.quota = (SELECT MAX(quota) FROM company WHERE cid IN (SELECT cid FROM apply WHERE sid = %s))
            AND d.quota = (SELECT MIN(quota) FROM company WHERE cid IN (SELECT cid FROM apply WHERE sid = %s))
        """, (sid, sid, sid,))
        company_with_max_and_min_quota_fetched = cursor.fetchone()
        if company_with_max_and_min_quota_fetched:
            company_with_max_quota = round(company_with_max_and_min_quota_fetched['company_with_max_quota'], 2)
            company_with_min_quota = round(company_with_max_and_min_quota_fetched['company_with_min_quota'], 2)
        else:
            company_with_max_quota, company_with_min_quota = None, None

        return render_template('summary.html',
            company_list=company_list,
            min_gpat=min_gpat,
            max_gpat=max_gpat,
            cities_and_app_counts=cities_and_app_counts,
            company_with_max_quota=company_with_max_quota,
            company_with_min_quota=company_with_min_quota)
        
    else:
        return redirect(url_for('login'))
        
        
@app.route('/logout')
def logout():
    if 'loggedin' in session and session['loggedin']:
        session.pop('loggedin', None)
        session.pop('userid', None)
        session.pop('username', None)

    return redirect(url_for('login'))
    

if __name__ == '__main__':
    port = int(os.environ.get('PORT', 5000))
    app.run(debug=True, host='0.0.0.0', port=port)
