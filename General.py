import streamlit as st 
import faker as fk

my_faker = fk.Faker()

st.title("Temas Generales")

st.text("En este apartado de la página se pondrá algunos ejemplos, planificación del curso y algunos asuntos de índole general")

st.image(my_faker.image((900,400)))