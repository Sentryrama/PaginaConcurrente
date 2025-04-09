import streamlit as st
import faker as fk

my_faker = fk.Faker()

st.title("Seccion de presentaciones")

st.image(my_faker.image((900,400)))
