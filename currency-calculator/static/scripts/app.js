angular.module('cinkciarzApp', ['ngRoute'])
    .config(['$routeProvider', function ($routeProvider) {
        $routeProvider.when('/', {
            templateUrl: 'templates/dashboard.html',
            controller: 'MainController'
        }).otherwise({
            redirectTo: '/'
        });
}]);

angular.module('cinkciarzApp')
    .controller('MainController',['$scope', '$http', function ($scope, $http) {
        $scope.currencyTileAmout = 10;
        $scope.selectedCurrencyCourse = undefined;
        $scope.transactions = [];
        $scope.selectedCurrencyAmount = 0;

        $scope.showMoreCurrencies = function () {
            $scope.currencyTileAmout += 10;
        };

        $scope.selectedCurrencyAmountChange = function () {
            var rates = [$scope.selectedCurrency.Currency.ratesA,
                $scope.selectedCurrency.Currency.ratesB,
                $scope.selectedCurrency.Currency.ratesC
            ];

            rates.sort((a, b) => {
                return a['amount'] - b['amount'];
            });

            console.log(rates);

            for(var i = 0; i < rates.length; i++) {
                if($scope.selectedCurrencyAmount < rates[i].amount) {
                    $scope.selectedCurrencyCourse = rates[i]["1"];
                    return;
                }
            }
            $scope.selectedCurrencyCourse = rates[2]["1"];
        };

        $scope.selectedCurrencyUpdate = function () {
            $scope.selectedCurrencyCourse = $scope.selectedCurrency.Currency.ratesA['1'];
        };

        $scope.fetchTransactions = function () {
            $http({method: 'GET', url: 'transactions'}).then(function (resp) {
                $scope.transactions = resp.data.transactions.map(function (transactionSerialize) {
                    var splittedTransaction = transactionSerialize.split(' ');

                    return {
                        currency: splittedTransaction[0],
                        amount: splittedTransaction[1],
                        rate: splittedTransaction[2],
                        profit: splittedTransaction[3]
                    };
                });
                $scope.transactionsSumProfit = resp.data.transactionsSumProfit;
            });
        };

	$scope.getCurrencyAmount = function (currencyName, trans) {
            for(var i=0; i<trans.length; ++i) if(trans[i].currency == currencyName) return trans[i].amount;
            return trans[0].amount;
	}
        $scope.addCurrencyTransaction = function (currencyName, amount, rate, posAmount=undefined) {
            console.log(currencyName, amount, rate);
            if(amount == null) amount = 0;
            if(rate == null) rate = 0;
            if(posAmount != undefined && amount < posAmount) if(!window.confirm("Uwaga! Zarządzono sprzedaż większej ilości waluty ("+-amount+"), niż na stanie ("+posAmount+")! Kontynuować?")) return;
            $http({
                method: 'POST',
                url: 'addCurrencyTransaction',
                data: currencyName + ' ' + amount + ' ' + rate
            }).then(function () {
                $scope.fetchTransactions();
            });
        };
        
        $scope.sellCurrency = function (currencyName, amount, rate) {
            alert('To trzeba zaimplementować :)');
        };

        window.test = $http;

        $scope.fetchTransactions();

        $http.get('currencies').then(function (response) {
            $scope.currencies = response.data.currencies;
            $scope.selectedCurrency = $scope.currencies[0];
            $scope.selectedCurrencyUpdate();
        });
    }]);
