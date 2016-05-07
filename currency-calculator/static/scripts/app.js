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

        $scope.showMoreCurrencies = function () {
            $scope.currencyTileAmout += 10;
        };

        $scope.selectedCurrencyUpdate = function () {
            console.log($scope.selectedCurrency);
            $scope.selectedCurrencyCourse = $scope.selectedCurrency.Currency.ratesA['1'];
        };

        $http.get('currencies').then(function (response) {
            $scope.currencies = response.data.currencies;
            $scope.selectedCurrency = $scope.currencies[0];
            $scope.selectedCurrencyUpdate();
        });
    }]);