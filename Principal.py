import streamlit as st 
import faker as fk

my_faker = fk.Faker()

st.set_page_config(
    page_title="Concurrente",
    page_icon=":material/arrow_or_edge:",
    layout="wide",
)

st.title("Página Principal")

st.text("En esta página se pondrán las prácticas y el material teórico que se revise a lo largo del curso.")

st.image(my_faker.image((900,400)))