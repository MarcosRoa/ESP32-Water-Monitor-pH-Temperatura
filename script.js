const tempEl = document.getElementById("temp");
const phEl = document.getElementById("ph");

const ctx = document.getElementById('grafico').getContext('2d');
const grafico = new Chart(ctx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: "pH",
            data: [],
            borderWidth: 2
        }]
    },
    options: {
        responsive: true
    }
});

function atualizar() {
    fetch("/data")
    .then(r => r.json())
    .then(d => {
        tempEl.innerHTML = d.temperatura + " °C";
        phEl.innerHTML = d.ph;

        grafico.data.labels.push(d.hora);
        grafico.data.datasets[0].data.push(d.ph);
        grafico.update();
    });
}

setInterval(atualizar, 1000);
