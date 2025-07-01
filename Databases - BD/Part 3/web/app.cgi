#!/usr/bin/python3
from wsgiref.handlers import CGIHandler
from flask import Flask
from flask import render_template, request, redirect, url_for
import psycopg2
import psycopg2.extras

## SGBD configs
DB_HOST = "db.tecnico.ulisboa.pt"
DB_USER = "ist187670"
DB_DATABASE = DB_USER
DB_PASSWORD = "nxaq6721"
DB_CONNECTION_STRING = "host=%s dbname=%s user=%s password=%s" % (
    DB_HOST,
    DB_DATABASE,
    DB_USER,
    DB_PASSWORD,
)

app = Flask(__name__)

@app.route('/')
def index():
  try:
    return render_template("index.html", params=request.args)
  except Exception as e:
    return str(e)

@app.route('/menu')
def menu_principal():
  try:
    return render_template("index.html", params=request.args)
  except Exception as e:
    return str(e)

@app.route('/erro')
def erro():
  try:
    return render_template("erro.html", params=request.args)
  except Exception as e:
    return str(e)

#####################################################################################
              #CATEGORIAS
#####################################################################################
@app.route('/categorias')
def listar_categorias():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM categoria;"
    cursor.execute(query)
    return render_template("categorias.html", cursor=cursor, params=request.args)
  except Exception as e:
    return str(e) 
  finally:
    cursor.close()
    dbConn.close()

@app.route('/categorias/inserir_categoria')
def inserir_categoria():
  try:
    return render_template("inserir_categoria.html", params=request.args)
  except Exception as e:
    return str(e)

@app.route('/categorias/execute_insert', methods=["POST"])
def insert_category():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "INSERT INTO categoria VALUES (%s);"
    data = (request.form["nome"],)
    cursor.execute(query,data)
    return redirect(url_for('listar_categorias'))
  except Exception as e:
    return render_template('erro.html', error_message = e)
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()

@app.route('/categorias/remover')
def remover_categoria():
  try:
    return render_template("remover_categoria.html", params=request.args)
  except Exception as e:
    return str(e)

@app.route('/categorias/perform_delete', methods=["POST"])
def delete_category():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "DELETE FROM categoria WHERE nome=%s;"
    data = (request.form["nome"],)
    cursor.execute(query,data)
    return redirect(url_for('listar_categorias'))
  except Exception as e:
    return render_template('erro.html', error_message = e)
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()

#####################################################################################
              #RETALHISTAS
#####################################################################################
@app.route('/retalhistas')
def listar_retalhistas():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM retalhista;"
    cursor.execute(query)
    return render_template("retalhistas.html", cursor=cursor, params=request.args)
  except Exception as e:
    return str(e) 
  finally:
    cursor.close()
    dbConn.close()

@app.route('/retalhistas/inserir_retalhista')
def inserir_retalhista():
  try:
    return render_template("inserir_retalhista.html", params=request.args)
  except Exception as e:
    return str(e)

@app.route('/retalhistas/execute_insert', methods=["POST"])
def insert_retailer():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    
    query = "INSERT INTO retalhista VALUES (%s, %s);"
    data = (request.form["tin"], request.form["name"])
    cursor.execute(query,data)

    return redirect(url_for('listar_retalhistas'))
  except Exception as e:
    return render_template('erro.html', error_message = e)
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()

@app.route('/retalhistas/remover')
def remover_retalhista():
  try:
    return render_template("remover_retalhista.html", params=request.args)
  except Exception as e:
    return str(e)

@app.route('/retalhistas/perform_delete', methods=["POST"])
def delete_retailer():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "DELETE FROM retalhista WHERE tin=%s;"
    data = (request.form["tin"],)
    cursor.execute(query,data)
    return redirect(url_for('listar_retalhistas'))
  except Exception as e:
    return render_template('erro.html', error_message = e)
  finally:
    dbConn.commit()
    cursor.close()
    dbConn.close()

#####################################################################################
              #EVENTOS REPOSICAO IVM
#####################################################################################
@app.route('/eventos')
def listar_eventos():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM evento_reposicao;"
    cursor.execute(query)
    return render_template("eventos.html", cursor=cursor, params=request.args)
  except Exception as e:
    return str(e) 
  finally:
    cursor.close()
    dbConn.close()

#####################################################################################
              #Super Categorias
#####################################################################################
@app.route('/supercategorias')
def listar_supercategorias():
  dbConn=None
  cursor=None
  try:
    dbConn = psycopg2.connect(DB_CONNECTION_STRING)
    cursor = dbConn.cursor(cursor_factory = psycopg2.extras.DictCursor)
    query = "SELECT * FROM super_categoria;"
    cursor.execute(query)
    return render_template("supercategorias.html", cursor=cursor, params=request.args)
  except Exception as e:
    return str(e) 
  finally:
    cursor.close()
    dbConn.close()

CGIHandler().run(app)