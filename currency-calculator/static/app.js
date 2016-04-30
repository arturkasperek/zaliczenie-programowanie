$.ajax({
    url: "currencies",
    type: "GET",
    success: function (response) {
        var currencies = JSON.parse(response);
        
        currencies.currencies.forEach(function (currency) {
            var currency = currency.Currency,
                rateA = currency.ratesA;

            console.log(currency);
            $('.container').append(
                `<div style="padding: 2em">
                    <span>${currency.name}</span></br>
                    <span>Kurs do ${rateA.amount} ${currency.name}: </span></br>
                    <span>Kupno  Sprzedarz</span></br>
                    <span>${rateA["1"]} ${rateA["2"]}</span>
                </div>`);
        });
    },
    error: function (xhr, status) {
        alert("error");
    }
});