from fastapi import FastAPI
from pydantic import BaseModel
from typing import Optional # Importe o Optional

# Cria um modelo de dados atualizado
class DeviceData(BaseModel):
    button_id: str
    status: str
    temperature: Optional[float] = None

app = FastAPI()

@app.get("/")
def read_root():
    return {"Status": "Server is running"}

# O endpoint usa DeviceData
@app.post("/button_status")
async def receive_device_data(data: DeviceData):
    print(f"Recebido: Botão {data.button_id} foi {data.status}", end="")

    # Verifica se o dado de temperatura foi enviado
    if data.temperature is not None:
        print(f", Temperatura: {data.temperature:.2f}°C")
    else:
        print() # ir para a proxima linha <-- não descobri outra forma

    return {"message": "Data received successfully"}
