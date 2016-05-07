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
            });
        };

        $scope.addCurrencyTransaction = function (currencyName, amount, rate) {
            console.log(currencyName, amount, rate);
            if(amount == null) amount = 0;
            if(rate == null) rate = 0;
            $http({
                method: 'POST',
                url: 'addCurrencyTransaction',
                data: currencyName + ' ' + amount + ' ' + rate
            }).then(function () {
                $scope.fetchTransactions();
            });
        };

        window.test = $http;

        $scope.fetchTransactions();

        $http.get('currencies').then(function (response) {
            $scope.currencies = response.data.currencies;
            $scope.selectedCurrency = $scope.currencies[0];
            $scope.selectedCurrencyUpdate();
        });
    }]);