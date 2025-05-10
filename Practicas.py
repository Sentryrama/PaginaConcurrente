import streamlit as st
import faker as fk
from streamlit_pdf_viewer import pdf_viewer

my_faker = fk.Faker()

st.title("Seccion de Practicas")

cols = st.columns(2, vertical_alignment="bottom")

with cols[0]:
    num = st.selectbox("Selecciona la practica", range(1,11))


path_to_pdf = f"assets/Practicas/Practica{num}.pdf"
with cols[1]:

    with open(path_to_pdf, "rb") as file:
        st.download_button(
            label=f"Practica {num}",
            data=file,
            file_name=f"Practica{num}.pdf",
            icon=":material/download:",
            mime="application/pdf",
            use_container_width=True

        )

pdf_viewer(
    path_to_pdf,
    width=900,
    height=1000,
    pages_to_render= list(range(1,10)),
)

