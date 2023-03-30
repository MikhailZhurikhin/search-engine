//
// Created by Mihail on 14.03.2023.
//

#ifndef SEARCH_ENGINE_STATUS_H
#define SEARCH_ENGINE_STATUS_H

#pragma once

/// Набор различных состояний
enum status {
    OK,     ///< результат обработки данных валиден
    ERROR   ///< результат обработки данных невалиден либо при обработке произошло выбрасывание исключения
};

#endif //SEARCH_ENGINE_STATUS_H
