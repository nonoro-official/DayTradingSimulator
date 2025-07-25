//
// Created by sonik on 7/24/2025.
//

#include "Stock.h"

float Stock::GetShareValue() { return shares * company->GetCurrentPrice(); }