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
        $scope.historyModeOn = false;

        function getTransactionCurrentSellPrice(amount, currencySymbol) {
            var toRet;
            $scope.currencies.forEach(function (currencyInfoObj) {
                if(currencyInfoObj.Currency.name == currencySymbol) {
                    if(amount < currencyInfoObj.Currency.ratesA.amount) {
                        toRet= currencyInfoObj.Currency.ratesA['2']
                    } else if(amount < currencyInfoObj.Currency.ratesB.amount) {
                        toRet= currencyInfoObj.Currency.ratesB['2']
                    } else {
                        toRet= currencyInfoObj.Currency.ratesC['2'];
                    }
                }
            });

            return toRet;
        }

        $scope.showMoreCurrencies = function () {
            $scope.currencyTileAmout += 10;
        };

        $scope.selectedCurrencyAmountChange = function (selectedCurrencyCourse, calcOnlyProfitable) {
            var rates = [$scope.selectedCurrency.Currency.ratesA,
                $scope.selectedCurrency.Currency.ratesB,
                $scope.selectedCurrency.Currency.ratesC
            ];

            function calcProfitable(primaryCourse) {
                if (primaryCourse) {
                    $scope.profitable = primaryCourse > parseFloat($scope.currentSellTransaction.middleRate);
                } else {
                    $scope.profitable = true;
                }
            }

            if(calcOnlyProfitable) {
                calcProfitable(selectedCurrencyCourse);
                return;
            }

            rates.sort((a, b) => {
                return a['amount'] - b['amount'];
            });

            for(var i = 0; i < rates.length; i++) {
                if($scope.selectedCurrencyAmount < rates[i].amount) {
                    $scope.selectedCurrencyCourse = rates[i]["1"];
                    calcProfitable($scope.selectedCurrencyCourse);
                    return;
                }
            }
            calcProfitable($scope.selectedCurrencyCourse);
            $scope.selectedCurrencyCourse = rates[2]["1"];
        };

        $scope.selectedCurrencyUpdate = function () {
            $scope.selectedCurrencyCourse = $scope.selectedCurrency.Currency.ratesA['1'];
        };

        $scope.fetchTransactions = function () {
            function calcCurrencysProfits(transactionsByCurrency) {
                for(var key in transactionsByCurrency) {
                    var currencyObj = transactionsByCurrency[key],
                        profit = 0,
                        amount = 0,
                        investSum = 0;
                    currencyObj.transactions.forEach(function (transaction) {
                        profit += parseFloat(transaction.profit);
                        amount += parseInt(transaction.amount);
                        investSum += parseInt(transaction.amount) * parseFloat(transaction.rate);
                    });
                    currencyObj.profit = parseFloat(profit.toFixed(4));
                    currencyObj.amount = amount;
                    currencyObj.middleRate = (investSum / amount).toFixed(4);
                }
            }
            $scope.addTransactionCompleted = false;
            $http({method: 'GET', url: 'transactions'}).then(function (resp) {
                $scope.addTransactionCompleted = true;
                $scope.transactions = resp.data.transactions.map(function (transactionSerialize, id) {
                    var splittedTransaction = transactionSerialize.split(' ');

                    return {
                        currency: splittedTransaction[0],
                        amount: splittedTransaction[1],
                        rate: splittedTransaction[2],
                        profit: parseFloat(splittedTransaction[3]).toFixed(4),
                        id: id,
                    };
                });

                $scope.transactions = $scope.transactions.filter(function (transaction) {
                    return transaction.amount > 0;
                });

                $scope.transactionsByCurrency = $scope.transactions.reduce(function(previousValue, currentValue) {
                    var currencyObj = previousValue[currentValue.currency];
                    if(currencyObj) {
                        currencyObj.transactions.push(currentValue);
                    } else {
                        previousValue[currentValue.currency] = {
                            transactions: [currentValue],
                            currencySymbol: currentValue.currency
                        };
                    }

                    return previousValue;
                }, {});

                calcCurrencysProfits($scope.transactionsByCurrency);
            });
            
            $http({
                method: 'GET',
                url: 'sellHistory'
            }).then(function (resp) {
                var sellTransactions = resp.data.split('\n');

                sellTransactions = sellTransactions.filter(function (sellHistoryTransaction) {
                    return sellHistoryTransaction.length > 0;
                });

                $scope.historySell = sellTransactions.map(function (transaction) {
                    var splittedTransaction = transaction.split(' ');
                    var sell = parseFloat(splittedTransaction[2]);
                    var buy = parseFloat(splittedTransaction[3]);
                    var amount = parseInt(splittedTransaction[1]);
                    return {
                        symbol: splittedTransaction[0],
                        amount: amount,
                        sell: sell.toFixed(4),
                        buy: buy.toFixed(4),
                        profit: ((sell - buy) * amount).toFixed(4)
                    };
                });
                $scope.transactionsSumProfit = $scope.historySell.reduce(function (prevVal, currVal) {
                    return prevVal + parseFloat(currVal.profit)
                }, 0);
            })
        };

        $scope.addCurrencyTransaction = function (currencyName, amount, rate) {
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

        $scope.onCurrencyTileClick = function (currencyObj) {
            $scope.managedCurrency = currencyObj;
        };

        $scope.hideSecondModal = function () {
            $('#sell-currency-modal').modal('hide');
        };

        $scope.hideFirstModal = function () {
            $('#currency-manager-modal').modal('hide');
        };

        $scope.sellCurrency = function(amount, sellPrice) {
            if( $scope.currentSellTransaction.amount < amount ) {
                alert("Nie masz tyle waluty!");
            } else {
                $http({
                    method: 'POST',
                    url: 'sellCurrency',
                    data: $scope.currentSellTransaction.id + ' ' + amount + ' ' + sellPrice + ' ' + $scope.currentSellTransaction.rate
                }).then(function () {
                    $scope.fetchTransactions();
                    $('#sell-currency-modal').modal('hide');
                    $('#currency-manager-modal').modal('hide');
                });
            }
        };

        $scope.selectCurrentSellTransaction = function (transaction) {
            $scope.currentSellTransaction=transaction;

            $scope.currencies.forEach(function (currencyObj) {
                if(currencyObj.Currency.name == $scope.managedCurrency.currencySymbol) {
                    $scope.selectedCurrency = currencyObj;
                }
            })
        };

        $scope.onTileSellClick = function (e, transaction) {
            e.stopPropagation();
            e.preventDefault();
            $('#sell-currency-two').modal('show');

            $scope.currentSellTransaction=transaction;

            $scope.currencies.forEach(function (currencyObj) {
                if(currencyObj.Currency.name == transaction.currencySymbol) {
                    $scope.selectedCurrency = currencyObj;
                }
            });
            $scope.selectedCurrencyAmountChange();
        };

        $scope.sellCurrencyAll = function(selectedCurrencyAmount, selectedCurrencyCourse) {
            var currencyTransactions = $scope.transactions.filter(function (transaction) {
                return $scope.selectedCurrency.Currency.name == transaction.currency;
            });

            var count = currencyTransactions.reduce(function (prev, curr) {
                return prev + parseInt(curr.amount);
            }, 0);

            if(selectedCurrencyAmount > count) {
                alert("Nie masz tyle waluty!");
            } else {
                currencyTransactions.sort(function (a, b) {
                    return parseFloat(a.rate) < parseFloat(b.rate);
                });

                currencyTransactions.forEach(function (transaction) {
                    if(selectedCurrencyAmount > 0) {
                        var amountToSell = 0;
                        if(transaction.amount > selectedCurrencyAmount) {
                            amountToSell = selectedCurrencyAmount;

                            selectedCurrencyAmount = 0;
                        } else {
                            amountToSell = transaction.amount;
                            selectedCurrencyAmount -= amountToSell;
                        }
                        $http({
                            method: 'POST',
                            url: 'sellCurrency',
                            data: transaction.id + ' ' + amountToSell + ' ' + selectedCurrencyCourse + ' ' + transaction.rate
                        }).then(function () {
                            $scope.fetchTransactions();
                            $('#sell-currency-two').modal('hide');
                        });
                    }
                });
            }
        };

        $scope.fetchTransactions();

        $http.get('currencies').then(function (response) {
            $scope.currencies = response.data.currencies;
            $scope.selectedCurrency = $scope.currencies[0];
            $scope.selectedCurrencyUpdate();
        });
    }]);