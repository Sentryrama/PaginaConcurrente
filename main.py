import streamlit as st 

paginas = st.navigation(
    {"Curso 25-I":[
        st.Page("Principal.py", title="Página Principal"),
        st.Page("Videos.py", title="Videos"),
        st.Page("Practicas.py", title="Practicas"),
        st.Page("Presentaciones.py", title="Presentaciones"),
        st.Page("Examenes.py", title="Examenes Semanales"),
        st.Page("General.py", title="Temas Generales"),
        ]
    },
    position="sidebar"
    )

paginas.run()