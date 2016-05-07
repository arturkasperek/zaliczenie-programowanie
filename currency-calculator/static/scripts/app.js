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

        $scope.showMoreCurrencies = function () {
            $scope.currencyTileAmout += 10;
        };

        $scope.selectedCurrencyUpdate = function () {
            $scope.selectedCurrencyCourse = $scope.selectedCurrency.Currency.ratesA['1'];
        };

        $scope.fetchTransactions = function () {
            $http({method: 'GET', url: 'transactions'}).then(function (resp) {
                $scope.transactions = resp.data.transactions.map(function (transactionSerialize) {
                    var splitedTransaction = transactionSerialize.split(' ');

                    return {
                        currency: splitedTransaction[0],
                        amount: splitedTransaction[1],
                        rate: splitedTransaction[2]
                    };
                });
            });
        };

        $scope.addCurrencyTransaction = function (currencyName, amount, rate) {
            console.log(currencyName, amount, rate);
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